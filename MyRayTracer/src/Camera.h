#pragma once

#include "Hittable.h"
#include "Material.h"

#include <fstream>

class Camera
{
public:
	int Width = 400;
	int Height = 225;
	int SPP = 10; // Count of random samples for each pixel (Sample Per Pixel)
	int maxDepth = 10; // Maximum number of ray bounces into scene

	float vfov = 90.0f; // Vertical field of view
	vec3 lookFrom = vec3(0.0f, 0.0f, 0.0f); // Point camera is looking from
	vec3 lookAt = vec3(0.0f, 0.0f, -1.0f);	// Point camera is looking at
	vec3 vUp = vec3(0.0f, 1.0f, 0.0f);		// Camera-relative "up" direction

	float defocusAngle = 0.0f; // Variation angle of rays through each pixel
	float focusDist = 10.0f;   // Distance from camera lookFrom point to plane of perfect focus

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
				color pixel_color(0.0f);
				for (int sample = 0; sample < SPP; ++sample) {
					Ray ray = getRay(i, j);
					pixel_color += castRay(ray, world, maxDepth);
				}
				write_color(out, pixelSamplesScale * pixel_color);
			}
		}
		std::clog << "\rDone.                 \n";
		out.close();
	}

private:
	void Initialize() {
		pixelSamplesScale = 1.0f / SPP;

		center = lookFrom;

		// Determine viewport dimensions.
		float focal_length = (lookAt - lookFrom).norm();
		float theta = Radians(vfov);
		float h = std::tanf(theta / 2.0f);
		float viewport_height = 2.0f * h * focusDist;
		float viewport_width = viewport_height * aspect_ratio;

		// Calculate the u,v,w unit basis vectors for the camera coordinate frame.
		w = (lookFrom - lookAt).normalized();
		u = cross(vUp, w).normalized();
		v = cross(w, u);

		// Calculate the vectors across the horizontal and down the vertical viewport edges.
		vec3 viewport_u = viewport_width * u;
		vec3 viewport_v = viewport_height * -v;

		// Calculate the horizontal and vertical delta vectors from pixel to pixel.
		pixel_delta_u = viewport_u / (float)Width;
		pixel_delta_v = viewport_v / (float)Height;

		// Calculate the location of the upper left pixel.
		vec3 viewport_upper_left = lookFrom - (focusDist * w) - viewport_u / 2.0f - viewport_v / 2.0f;
		pixel00_loc = viewport_upper_left + 0.5f * (pixel_delta_u + pixel_delta_v);

		// Calculate the camera defocus disk basis vectors.
		vec3 defocus_radius = focusDist * std::tanf(Radians(defocusAngle / 2));
		defocus_disk_u = u * defocus_radius;
		defocus_disk_v = v * defocus_radius;
	}

	Ray getRay(int i, int j) const {
		// Construct a camera ray originating from the defocus disk and directed at randomly
		// sampled point around the pixel location i, j.

		vec3 offset = sampleSquare();
		vec3 pixelSample = pixel00_loc +
			((i + offset.x) * pixel_delta_u) +
			((j + offset.y) * pixel_delta_v);

		vec3 rayOrig = (defocusAngle <= 0.0f) ? center : defocusDiskSample();
		vec3 rayDir = pixelSample - rayOrig;

		return Ray(rayOrig, rayDir);
	}

	vec3 sampleSquare() const {
		// Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
		return vec3(randomFloat() - 0.5f, randomFloat() - 0.5f, 0.0f);
	}

	vec3 defocusDiskSample() const {
		// Returns a random point in the camera defocus disk.
		vec3 p = randomInUnitDisk();
		return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
	}

	color castRay(const Ray& ray, const Hittable& world, int depth) const {
		if (depth <= 0)
			return color(0.0f);

		HitPayload payload;
		if (world.hit(ray, Interval(0.001f, infinity), payload)) {
			Ray scattered;
			color attenuation;
			if (payload.material->Scatter(ray, payload, attenuation, scattered))
				return attenuation * castRay(scattered, world, depth - 1);
			return color(0.0f);
		}

		vec3 unit_dir = ray.GetDirection().normalized();
		float a = 0.5f * (unit_dir.y + 1.0f);
		return (1.0f - a) * color(1.0f) + a * color(0.5f, 0.7f, 1.0f);
	}

private:
	float aspect_ratio = (float)Width / Height;
	float pixelSamplesScale; // Color scale factor for a sum of pixel samples
	vec3 center;
	vec3 pixel00_loc;
	vec3 pixel_delta_u;
	vec3 pixel_delta_v;
	vec3 u, v, w; // Camera frame basis vectors
	vec3 defocus_disk_u; // Defocus disk horizontal radius
	vec3 defocus_disk_v; // Defocus disk vertical radius

};