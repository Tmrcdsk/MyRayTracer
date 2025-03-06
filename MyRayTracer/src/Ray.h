#pragma once

#include "Vector.h"

class Ray
{
public:
	Ray() = default;
	Ray(const vec3& o, const vec3& d) : orig(o), dir(d) {}

	const vec3& GetOrigin() const { return orig; }
	const vec3& GetDirection() const { return dir; }

	vec3 at(float t) const {
		return orig + t * dir;
	}

private:
	vec3 orig;
	vec3 dir;
};