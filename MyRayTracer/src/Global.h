#pragma once

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>

const float infinity = std::numeric_limits<float>::max();
const float PI = 3.14159265358979323846;

inline float Radians(float degrees) {
	return degrees * PI / 180.0f;
}

#include "Color.h"
#include "Ray.h"
#include "Vector.h"
#include "Interval.h"