#pragma once

#include "Global.h"
#include "AABB.h"

class Material;

struct HitPayload
{
	vec3 p;
	vec3 normal;
	std::shared_ptr<Material> material;
	float t;
	float u, v;
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

	virtual AABB boundingBox() const = 0;
};

class Translate : public Hittable
{
public:
	Translate(std::shared_ptr<Hittable> object, const vec3& offset)
		: object(object), offset(offset) {
		bbox = object->boundingBox() + offset;
	}

	bool hit(const Ray& ray, Interval t, HitPayload& payload) const override {
		// Move the ray backwards by the offset
		Ray offsetRay(ray.GetOrigin() - offset, ray.GetDirection(), ray.time());

		// Determine whether an intersection exists along the offset ray (and if so, where)
		if (!object->hit(offsetRay, t, payload))
			return false;

		// Move the intersection point forwards by the offset
		payload.p += offset;

		return true;
	}

	AABB boundingBox() const override { return bbox; }

private:
	std::shared_ptr<Hittable> object;
	vec3 offset;
	AABB bbox;
};
