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

class RotateY : public Hittable
{
public:
	RotateY(std::shared_ptr<Hittable> object, float angle) : object(object) {
		auto radians = Radians(angle);
		sinTheta = sin(radians);
		cosTheta = cos(radians);
		bbox = object->boundingBox();

		vec3 min(infinity, infinity, infinity);
		vec3 max(-infinity, -infinity, -infinity);

		// Update the bounding box by considering the rotation of each of its corners
		for (int i = 0; i < 2; ++i) {
			for (int j = 0; j < 2; ++j) {
				for (int k = 0; k < 2; ++k) {
					auto x = i * bbox.x.max + (1 - i) * bbox.x.min;
					auto y = j * bbox.y.max + (1 - j) * bbox.y.min;
					auto z = k * bbox.z.max + (1 - k) * bbox.z.min;

					auto newx = cosTheta * x + sinTheta * z;
					auto newz = -sinTheta * x + cosTheta * z;

					vec3 tester(newx, y, newz);

					for (int c = 0; c < 3; ++c) {
						min[c] = std::fmin(min[c], tester[c]);
						max[c] = std::fmax(max[c], tester[c]);
					}
				}
			}
		}

		bbox = AABB(min, max);
	}

	bool hit(const Ray& ray, Interval t, HitPayload& payload) const override {
		// Transform the ray from world space to object space.
		auto origin = vec3(
			(cosTheta * ray.GetOrigin().x - sinTheta * ray.GetOrigin().z),
			ray.GetOrigin().y,
			(sinTheta * ray.GetOrigin().x + cosTheta * ray.GetOrigin().z)
		);

		auto direction = vec3(
			(cosTheta * ray.GetDirection().x - sinTheta * ray.GetDirection().z),
			ray.GetDirection().y,
			(sinTheta * ray.GetDirection().x + cosTheta * ray.GetDirection().z)
		);

		Ray rotatedRay(origin, direction, ray.time());

		// Determine whether an intersection exists in object space (and if so, where).
		if (!object->hit(rotatedRay, t, payload))
			return false;
		
		// Transform the intersection from object space back to world space.
		auto p = vec3(
			(cosTheta * payload.p.x + sinTheta * payload.p.z),
			payload.p.y,
			(-sinTheta * payload.p.x + cosTheta * payload.p.z)
		);

		payload.normal = vec3(
			(cosTheta * payload.normal.x + sinTheta * payload.normal.z),
			payload.normal.y,
			(-sinTheta * payload.normal.x + cosTheta * payload.normal.z)
		);

		return true;
	}

	AABB boundingBox() const override { return bbox; }

private:
	std::shared_ptr<Hittable> object;
	float sinTheta;
	float cosTheta;
	AABB bbox;
};
