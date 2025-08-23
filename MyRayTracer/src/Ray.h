#pragma once

#include "Vector.h"

class Ray
{
public:
	Ray() = default;
	Ray(const vec3& o, const vec3& d, double t)
		: orig(o), dir(d), tm(t) {}
	Ray(const vec3& o, const vec3& d)
		: Ray(o, d, 0) {}

	const vec3& GetOrigin() const { return orig; }
	const vec3& GetDirection() const { return dir; }

	double time() const { return tm; }

	vec3 at(float t) const {
		return orig + t * dir;
	}

private:
	vec3 orig;
	vec3 dir;
	double tm;
};