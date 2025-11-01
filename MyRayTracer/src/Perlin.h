#pragma once

#include "Global.h"

class Perlin
{
public:
	Perlin() {
		for (int i = 0; i < point_count; ++i) {
			randfloat[i] = randomFloat();
		}

		perlinGeneratePerm(perm_x);
		perlinGeneratePerm(perm_y);
		perlinGeneratePerm(perm_z);
	}

	float noise(const vec3& p) const {
		auto i = int(4 * p.x) & 255;
		auto j = int(4 * p.y) & 255;
		auto k = int(4 * p.z) & 255;
		
		return randfloat[perm_x[i] ^ perm_y[j] ^ perm_z[k]];
	}

private:
	static const int point_count = 256;
	float randfloat[point_count];
	int perm_x[point_count];
	int perm_y[point_count];
	int perm_z[point_count];

	static void perlinGeneratePerm(int* p) {
		for (int i = 0; i < point_count; ++i)
			p[i] = i;

		permute(p, point_count);
	}

	static void permute(int* p, int n) {
		for (int i = n - 1; i > 0; --i) {
			int target = randomInt(0, i);
			int tmp = p[i];
			p[i] = p[target];
			p[target] = tmp;
		}
	}
};
