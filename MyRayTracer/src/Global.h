#pragma once

#include <cmath>
#include <random>
#include <iostream>
#include <limits>
#include <memory>

const float infinity = std::numeric_limits<float>::max();
const float PI = 3.14159265358979323846;

inline float Radians(float degrees) {
	return degrees * PI / 180.0f;
}

inline float randomFloat() {
	// Returns a random real in [0,1).
	static std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
	static std::mt19937 generator;
	return distribution(generator);
}

inline float randomFloat(float min, float max) {
	// Returns a random real in [min,max).
	return min + (max - min) * randomFloat();
}

#include "Color.h"
#include "Ray.h"
#include "Vector.h"
#include "Interval.h"