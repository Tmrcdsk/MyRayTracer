#include <iostream>
#include <fstream>

#include "Color.h"
#include "Vector.h"
#include "Ray.h"

color castRay(const Ray& ray) {
	vec3 unit_dir = ray.GetDirection().normalized();
	float a = 0.5f * (unit_dir.y + 1.0f);
	return (1.0f - a) * color(1.0f) + a * color(0.5f, 0.7f, 1.0f);
}

int main()
{
	int Width = 400;
	int Height = 225;
	float aspect_ratio = (float)Width / Height;

	// Camera
	float focal_length = 1.0f;
	float viewport_height = 2.0f;
	float viewport_width = viewport_height * aspect_ratio;
	vec3 camera_center(0.0f);

	// Calculate the vectors across the horizontal and down the vertical viewport edges.
	vec3 viewport_u = vec3(viewport_width, 0.0f, 0.0f);
	vec3 viewport_v = vec3(0.0f, -viewport_height, 0.0f);

	// Calculate the horizontal and vertical delta vectors from pixel to pixel.
	vec3 pixel_delta_u = viewport_u / (float)Width;
	vec3 pixel_delta_v = viewport_v / (float)Height;

	// Calculate the location of the upper left pixel.
	vec3 viewport_upper_left = camera_center - vec3(0.0f, 0.0f, focal_length) - viewport_u / 2.0f - viewport_v / 2.0f;
	vec3 pixel00_loc = viewport_upper_left + 0.5f * (pixel_delta_u + pixel_delta_v);

	// Render
	std::ofstream out("out.ppm");
	if (!out) {
		std::cerr << "Cannot create output file!\n";
		return -1;
	}
	out << "P3\n" << Width << " " << Height << "\n255\n";

	for (int j = 0; j < Height; ++j) {
		std::clog << "\rScanlines remaining: " << (Height - j) << ' ' << std::flush;
		for (int i = 0; i < Width; ++i) {
			vec3 pixel_center = pixel00_loc + ((float)i * pixel_delta_u) + ((float)j * pixel_delta_v);
			Ray ray(camera_center, pixel_center - camera_center);

			color pixel_color = castRay(ray);
			write_color(out, pixel_color);
		}
	}
	std::clog << "\rDone.                 \n";
	out.close();
	return 0;
}