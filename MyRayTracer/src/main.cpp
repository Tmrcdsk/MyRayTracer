#include <iostream>
#include <fstream>

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
		for (int i = 0; i < Width; ++i) {
			float r = (float)i / (Width - 1);
			float g = (float)j / (Height - 1);
			float b = 0.0f;

			int ir = (int)(r * 255);
			int ig = (int)(g * 255);
			int ib = (int)(b * 255);

			out << ir << " " << ig << " " << ib << "\n";
		}
	}
	out.close();
	return 0;
}