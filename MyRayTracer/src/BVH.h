#pragma once

#include "Global.h"
#include "AABB.h"
#include "Hittable.h"
#include "HittableList.h"

class BVHNode : public Hittable
{
public:
	BVHNode(HittableList list) : BVHNode(list.objects, 0, list.objects.size()) {
		// There's a C++ subtlety here. This constructor (without span indices) creates an
		// implicit copy of the hittable list, which we will modify. The lifetime of the copied
		// list only extends until this constructor exits. That's OK, because we only need to
		// persist the resulting bounding volume hierarchy.
	}

	BVHNode(std::vector<std::shared_ptr<Hittable>>& objects, size_t start, size_t end) {
		// To be implemented later.
	}

	bool hit(const Ray& ray, Interval rayT, HitPayload& payload) const override {
		if (!bbox.hit(ray, rayT))
			return false;

		bool hitLeft = left->hit(ray, rayT, payload);
		bool hitRight = right->hit(ray, Interval(rayT.min, hitLeft ? payload.t : rayT.max), payload);

		return hitLeft || hitRight;
	}

	AABB boundingBox() const override { return bbox; }

private:
	std::shared_ptr<Hittable> left;
	std::shared_ptr<Hittable> right;
	AABB bbox;
};