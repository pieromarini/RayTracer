#include <atomic>
#include <cfloat>
#include <cstdlib>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include <SFML/Graphics.hpp>

#include "raytracer.h"
#include "timer.h"

constexpr unsigned WIDTH = 1024;
constexpr unsigned HEIGHT = 512;

constexpr unsigned N = 16;
constexpr unsigned N_SAMPLES = 5000;

constexpr unsigned W_CNT = (WIDTH + N - 1) / N;
constexpr unsigned H_CNT = (HEIGHT + N - 1) / N;

constexpr int maxDepth = 800;

const vec3 background = { 0.1, 0.1, 0.1 };

Hittable* world;

vec3 lookfrom = vec3(8.f, 1.9f, 6.5f);
vec3 lookat = vec3(1.f, 0.5f, 0.f);
vec3 vup = vec3(0.f, 1.f, 0.f);
float distToFocus = (lookfrom - lookat).magnitude() * 2.f;
float aperture = 0.08f;
Camera cam = Camera(lookfrom, lookat, vup, 20.f, float(WIDTH) / float(HEIGHT), aperture, distToFocus);

std::atomic<unsigned> doneCount;

class Pixels {
 public:
  Pixels(unsigned w, unsigned h)
	: width{ w },
	  height{ h },
	  data{ new float[width * height * 5]() },// RGBA + sample count
	  pixels{ new sf::Uint8[width * height * 4]() }// RGBA
  {}

  ~Pixels() {
	delete[] data;
	delete[] pixels;
  }

  sf::Uint8* get_pixels() {
	// convert accumulated pixels values so we can display them
	for (int i = 0; i < HEIGHT; i++)
	  for (int j = 0; j < WIDTH; j++) {
		const unsigned data_pos = (i * width + j) * 5;
		const unsigned pix_pos = ((HEIGHT - i - 1) * width + j) << 2;
		const float ns = data[data_pos + 4];// number of accumulated samples
		pixels[pix_pos + 0] =
		  sf::Uint8(255.99f * (sqrtf(data[data_pos + 0] / ns)));
		pixels[pix_pos + 1] =
		  sf::Uint8(255.99f * (sqrtf(data[data_pos + 1] / ns)));
		pixels[pix_pos + 2] =
		  sf::Uint8(255.99f * (sqrtf(data[data_pos + 2] / ns)));
		pixels[pix_pos + 3] = 255u;
	  }

	return pixels;
  }

  inline void accumulate(unsigned x, unsigned y, float r, float g, float b) {
	const unsigned pos = (y * width + x) * 5;
	data[pos + 0] += r;
	data[pos + 1] += g;
	data[pos + 2] += b;
	data[pos + 3] += 255.f;// opaque
	data[pos + 4] += 1.f;// number of samples
  }

  inline void accumulate(unsigned x, unsigned y, const vec3& col) {
	accumulate(x, y, col.x, col.y, col.z);
  }

  unsigned width;
  unsigned height;
  float* data;// RGBA + sample count

 private:
  sf::Uint8* pixels;
};

Pixels pixels{ WIDTH, HEIGHT };

struct Task {
  Task() : my_id{ id++ } {}

  Task(int x, int y) : sx{ x }, sy{ y }, my_id{ id++ } {}

  // snake pattern implementation
  void move_in_pattern(int& rx, int& ry) {
	static int x = -1, y = H_CNT - 1;
	static int dir = 0;

	x = dir ? x - 1 : x + 1;
	if (x == W_CNT || x == -1) {
	  x = y & 1 ? W_CNT - 1 : 0;
	  y--;
	  dir = !dir;
	}
	rx = x;
	ry = y;
  }

  bool get_next_task() {
	static bool taken[H_CNT][W_CNT] = {};
	static std::mutex m;

	std::lock_guard<std::mutex> guard{ m };

	bool found = false;
	int x, y;
	while (!found) {
	  move_in_pattern(x, y);
	  if (x < 0 || x > W_CNT || y < 0 || y > H_CNT) break;

	  if (!taken[y][x]) {
		sx = x * N;
		sy = y * N;
		taken[y][x] = true;
		found = true;
	  }
	}

	return found;
  }

  void operator()() {
	bool done = false;
	do {
	  if (!get_next_task()) {
		done = true;
		continue;
	  }

	  for (unsigned s = 0; s < N_SAMPLES; s++)
		for (unsigned y = sy; y < sy + N; y++)
		  for (unsigned x = sx; x < sx + N; x++) {
			if (x < 0 || y < 0 || x >= WIDTH || y >= HEIGHT) continue;

			const float u = float(x + drand48()) / float(WIDTH);
			const float v = float(y + drand48()) / float(HEIGHT);
			Ray r = cam.getRay(u, v);
			const vec3 col = rayColor(r, background, world, maxDepth);

			pixels.accumulate(x, y, col);
		  }
	} while (!done);

	doneCount++;

	std::cout << "Thread " << my_id << " is done!" << std::endl;
  }

  int sx = -1, sy = -1;
  int my_id;
  static int id;
};

int Task::id = 0;

int main() {
  sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Ray Tracing rules!", sf::Style::Titlebar | sf::Style::Close);
  sf::Texture tex;
  sf::Sprite sprite;

  if (!tex.create(WIDTH, HEIGHT)) {
	std::cerr << "[SFML] Error creating texture\n";
	return 1;
  }

  tex.setSmooth(false);

  sprite.setTexture(tex);

  world = randomScene();

  const auto n_threads = std::thread::hardware_concurrency();
  std::vector<std::thread> threads{ n_threads };

  Timer timer;

  for (auto& t : threads) t = std::thread{ Task{} };

  bool finishedRender = false;

  while (window.isOpen()) {
	sf::Event event;
	while (window.pollEvent(event)) {
	  if (event.type == sf::Event::Closed) window.close();
	}

	tex.update(pixels.get_pixels());

	window.clear();
	window.draw(sprite);
	window.display();

	if (!finishedRender && doneCount == n_threads) {
	  std::cout << "Finished rendering in " << timer.get_millis() << "ms\n";
	  finishedRender = true;
	}

	sf::sleep(sf::milliseconds(1000));
  }

  std::cout << "Waiting for all the threads to join\n";
  for (auto& t : threads) t.join();

  tex.copyToImage().saveToFile("out.png");
  std::cout << "Saved image to out.png\n";

  return 0;
}
