#pragma once

#include "Hittable.h"

#include <fstream>

class Camera
{
public:
	int Width = 400;
	int Height = 225;

	void render(const Hittable& world) {
		Initialize();

		std::ofstream out("out.ppm");
		if (!out) {
			std::cerr << "Cannot create output file!\n";
			return;
		}
		out << "P3\n" << Width << " " << Height << "\n255\n";

		for (int j = 0; j < Height; ++j) {
			std::clog << "\rScanlines remaining: " << (Height - j) << ' ' << std::flush;
			for (int i = 0; i < Width; ++i) {
				vec3 pixel_center = pixel00_loc + ((float)i * pixel_delta_u) + ((float)j * pixel_delta_v);
				Ray ray(center, pixel_center - center);

				color pixel_color = castRay(ray, world);
				write_color(out, pixel_color);
			}
		}
		std::clog << "\rDone.                 \n";
		out.close();
	}

private:
	void Initialize() {
		center = vec3(0.0f);

		// Determine viewport dimensions.
		float focal_length = 1.0f;
		float viewport_height = 2.0f;
		float viewport_width = viewport_height * aspect_ratio;

		// Calculate the vectors across the horizontal and down the vertical viewport edges.
		vec3 viewport_u = vec3(viewport_width, 0.0f, 0.0f);
		vec3 viewport_v = vec3(0.0f, -viewport_height, 0.0f);

		// Calculate the horizontal and vertical delta vectors from pixel to pixel.
		pixel_delta_u = viewport_u / (float)Width;
		pixel_delta_v = viewport_v / (float)Height;

		// Calculate the location of the upper left pixel.
		vec3 viewport_upper_left = center - vec3(0.0f, 0.0f, focal_length) - viewport_u / 2.0f - viewport_v / 2.0f;
		pixel00_loc = viewport_upper_left + 0.5f * (pixel_delta_u + pixel_delta_v);
	}

	color castRay(const Ray& ray, const Hittable& world) const {
		HitPayload payload;
		if (world.hit(ray, Interval(0.0f, infinity), payload)) {
			return 0.5f * (payload.normal + color(1.0f));
		}

		vec3 unit_dir = ray.GetDirection().normalized();
		float a = 0.5f * (unit_dir.y + 1.0f);
		return (1.0f - a) * color(1.0f) + a * color(0.5f, 0.7f, 1.0f);
	}

private:
	float aspect_ratio = (float)Width / Height;
	vec3 center;
	vec3 pixel00_loc;
	vec3 pixel_delta_u;
	vec3 pixel_delta_v;

};