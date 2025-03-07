#pragma once

#include "Global.h"

struct HitPayload
{
	vec3 p;
	vec3 normal;
	float t;
	bool front_face;

	void SetFaceNormal(const Ray& ray, const vec3& outwardNormal) {
		// Sets the hit record normal vector.
		// NOTE: the parameter `outward_normal` is assumed to have unit length.
		front_face = dot(ray.GetDirection(), outwardNormal) < 0.0f;
		normal = front_face ? outwardNormal : -outwardNormal;
	}
};

class Hittable
{
public:
	virtual ~Hittable() = default;

	virtual bool hit(const Ray& ray, Interval t, HitPayload& payload) const = 0;
};
