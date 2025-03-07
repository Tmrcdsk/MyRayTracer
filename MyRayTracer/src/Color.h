#pragma once

#include "Vector.h"
#include "Interval.h"

using color = vec3;

void write_color(std::ostream& out, const color& pixel_color) {
	auto r = pixel_color.x;
	auto g = pixel_color.y;
	auto b = pixel_color.z;

	// [0.0, 1.0] -> [0, 255]
	static const Interval intensity(0.000f, 0.999f);
	int rbyte = int(256 * intensity.clamp(r));
	int gbyte = int(256 * intensity.clamp(g));
	int bbyte = int(256 * intensity.clamp(b));

	out << rbyte << " " << gbyte << " " << bbyte << "\n";
}