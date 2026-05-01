#pragma once

#include "Global.h"

class AABB
{
public:
	Interval x, y, z;

	AABB() {} // The default AABB is empty, since intervals are empty by default.

	AABB(const Interval& x, const Interval& y, const Interval& z)
		: x(x), y(y), z(z) {
		padToMinimums();
	}

	AABB(const vec3& a, const vec3& b) {
		// Treat the two points a and b as extrema for the bounding box, so we don't require a
		// particular minimum/maximum coordinate order.
		x = (a[0] <= b[0]) ? Interval(a[0], b[0]) : Interval(b[0], a[0]);
		y = (a[1] <= b[1]) ? Interval(a[1], b[1]) : Interval(b[1], a[1]);
		z = (a[2] <= b[2]) ? Interval(a[2], b[2]) : Interval(b[2], a[2]);

		padToMinimums();
	}
	
	AABB(const AABB& box0, const AABB& box1) {
		x = Interval(box0.x, box1.x);
		y = Interval(box0.y, box1.y);
		z = Interval(box0.z, box1.z);
	}

	const Interval& axisInterval(int n) const {
		if (n == 1) return y;
		if (n == 2) return z;
		return x;
	}

	// rayT: [t_enter, t_exit]
	bool hit(const Ray& ray, Interval rayT) const {
		const vec3& rayOrig = ray.GetOrigin();
		const vec3& rayDir = ray.GetDirection();

		for (int axis = 0; axis < 3; ++axis) {
			const Interval& ax = axisInterval(axis);
			const float adinv = 1.0 / rayDir[axis];

			auto t0 = (ax.min - rayOrig[axis]) * adinv;
			auto t1 = (ax.max - rayOrig[axis]) * adinv;

			if (t0 < t1) {
				if (t0 > rayT.min) rayT.min = t0;
				if (t1 < rayT.max) rayT.max = t1;
			}
			else {
				if (t1 > rayT.min) rayT.min = t1;
				if (t0 < rayT.max) rayT.max = t0;
			}

			if (rayT.max <= rayT.min)
				return false;
		}
		return true;
	}

	int longestAxis() const {
		// Returns the index of the longest axis of the bounding box.
		if (x.size() > y.size())
			return x.size() > z.size() ? 0 : 2;
		else
			return y.size() > z.size() ? 1 : 2;
	}

	static const AABB empty, universe;

private:
	void padToMinimums() {
		// Adjust the AABB so that no side is narrower than some delta, padding if necessary.
		float delta = 0.0001f;
		if (x.size() < delta) x = x.expand(delta);
		if (y.size() < delta) y = y.expand(delta);
		if (z.size() < delta) z = z.expand(delta);
	}
};

const AABB AABB::empty    = AABB(Interval::empty, Interval::empty, Interval::empty);
const AABB AABB::universe = AABB(Interval::universe, Interval::universe, Interval::universe);

AABB operator+(const AABB& box, const vec3& offset) {
	return AABB(box.x + offset[0], box.y + offset[1], box.z + offset[2]);
}

AABB operator+(const vec3& offset, const AABB& bbox) {
	return bbox + offset;
}
