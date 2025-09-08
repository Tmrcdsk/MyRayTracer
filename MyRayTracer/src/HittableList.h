#pragma once

#include "Global.h"
#include "Hittable.h"
#include "AABB.h"

#include <vector>

class HittableList : public Hittable
{
public:
	std::vector<std::shared_ptr<Hittable>> objects;

	HittableList() = default;
	HittableList(std::shared_ptr<Hittable> object) { add(object); }

	void clear() { objects.clear(); }

	void add(std::shared_ptr<Hittable> object) {
		objects.push_back(object);
		bbox = AABB(bbox, object->boundingBox());
	}

	bool hit(const Ray& ray, Interval t, HitPayload& payload) const {
		HitPayload tmpPayload;
		bool hitAnything = false;
		float currentClosest = t.max;

		for (const auto& object : objects) {
			if (object->hit(ray, Interval(t.min, currentClosest), tmpPayload)) {
				hitAnything = true;
				currentClosest = tmpPayload.t;
				payload = tmpPayload;
			}
		}

		return hitAnything;
	}

	AABB boundingBox() const override { return bbox; }

private:
	AABB bbox;
};