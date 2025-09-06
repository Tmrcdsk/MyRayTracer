#pragma once

#include "Hittable.h"

class Sphere : public Hittable
{
public:
	// Stationary Sphere
	Sphere(const vec3& static_center, float radius, std::shared_ptr<Material> material) : center(static_center, vec3(0, 0, 0)), radius(std::fmax(0.0f, radius)), material(material) {}

	// Moving Sphere
	Sphere(const vec3& center1, const vec3& center2, float radius, std::shared_ptr<Material> material) : center(center1, center2 - center1), radius(std::fmax(0.0f, radius)), material(material) {}

	bool hit(const Ray& ray, Interval t, HitPayload& payload) const override {
		vec3 currentCenter = center.at(ray.time());
		vec3 oc = currentCenter - ray.GetOrigin();
		float a = ray.GetDirection().norm2();
		float h = dot(ray.GetDirection(), oc);
		float c = oc.norm2() - radius * radius;
		float discriminant = h * h - a * c;
		if (discriminant < 0)
			return false;

		float sqrtd = std::sqrtf(discriminant);

		float root = (h - sqrtd) / a;
		if (!t.surrounds(root)) {
			root = (h + sqrtd) / a;
			if (!t.surrounds(root))
				return false;
		}

		payload.p = ray.at(root);
		vec3 outwardNormal = (payload.p - currentCenter) / radius;
		payload.SetFaceNormal(ray, outwardNormal);
		payload.t = root;
		payload.material = material;

		return true;
	}

private:
	Ray center;
	float radius;
	std::shared_ptr<Material> material;
};