#include <iostream>
#include <fstream>

#include "Color.h"
#include "Vector.h"

int main()
{
	int Width = 256;
	int Height = 256;

	std::ofstream out("out.ppm");
	if (!out) {
		std::cerr << "Cannot create output file!\n";
		return -1;
	}
	out << "P3\n" << Width << " " << Height << "\n255\n";

	for (int j = 0; j < Height; ++j) {
		std::clog << "\rScanlines remaining: " << (Height - j) << ' ' << std::flush;
		for (int i = 0; i < Width; ++i) {
			auto pixel_color = color((float)i / (Width - 1), (float)j / (Height - 1), 0.0f);
			write_color(out, pixel_color);
		}
	}
	std::clog << "\rDone.                 \n";
	out.close();
	return 0;
}