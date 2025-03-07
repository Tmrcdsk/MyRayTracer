#pragma once

#include "Hittable.h"

class Sphere : public Hittable
{
public:
	Sphere(const vec3& center, float radius) : center(center), radius(radius) {}

	bool hit(const Ray& ray, float tmin, float tmax, HitPayload& payload) const override {
		vec3 oc = center - ray.GetOrigin();
		float a = ray.GetDirection().norm2();
		float h = dot(ray.GetDirection(), oc);
		float c = oc.norm2() - radius * radius;
		float discriminant = h * h - a * c;
		if (discriminant < 0)
			return false;

		float sqrtd = std::sqrtf(discriminant);

		float root = (h - sqrtd) / a;
		if (root <= tmin || root >= tmax) {
			root = (h + sqrtd) / a;
			if (root <= tmin || root >= tmax)
				return false;
		}

		payload.p = ray.at(root);
		vec3 outwardNormal = (payload.p - center) / radius;
		payload.SetFaceNormal(ray, outwardNormal);
		payload.t = root;

		return true;
	}

private:
	vec3 center;
	float radius;
};