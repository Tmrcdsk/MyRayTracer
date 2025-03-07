#pragma once

class Interval
{
public:
	Interval() : min(infinity), max(-infinity) {} // default interval is empty
	Interval(float min, float max) : min(min), max(max) {}

	float size() const { return max - min; }
	bool contains(float x) const { return min <= x && x <= max; }
	bool surrounds(float x) const { return min < x && x < max; }

	float clamp(float x) const {
		if (x < min)
			return min;
		if (x > max)
			return max;
		return x;
	}

	float min, max;
	static const Interval empty, universe;
};

const Interval Interval::empty = Interval(infinity, -infinity);
const Interval Interval::universe = Interval(-infinity, infinity);
