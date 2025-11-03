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
		auto u = p.x - std::floor(p.x);
		auto v = p.y - std::floor(p.y);
		auto w = p.z - std::floor(p.z);
		u = u * u * (3 - 2 * u);
		v = v * v * (3 - 2 * v);
		w = w * w * (3 - 2 * w);

		auto i = int(std::floor(p.x));
		auto j = int(std::floor(p.y));
		auto k = int(std::floor(p.z));
		float c[2][2][2];

		for (int di = 0; di < 2; ++di)
			for (int dj = 0; dj < 2; ++dj)
				for (int dk = 0; dk < 2; ++dk) {
					c[di][dj][dk] = randfloat[
						perm_x[(i + di) & 255] ^
						perm_y[(j + dj) & 255] ^
						perm_z[(k + dk) & 255]
					];
				}
		
		return trilinearInterp(c, u, v, w);
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

	static float trilinearInterp(float c[2][2][2], float u, float v, float w) {
		float accum = 0.0f;
		for (int i = 0; i < 2; ++i)
			for (int j = 0; j < 2; ++j)
				for (int k = 0; k < 2; ++k) {
					accum += (i * u + (1 - i) * (1 - u))
						   * (j * v + (1 - j) * (1 - v))
						   * (k * w + (1 - k) * (1 - w))
						   * c[i][j][k];
				}
		return accum;
	}
};
