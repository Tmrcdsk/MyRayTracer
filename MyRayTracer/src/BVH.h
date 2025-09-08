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
		// Build the bounding box of the span of source objects.
		bbox = AABB::empty;
		for (size_t objectIndex = start; objectIndex < end; ++objectIndex)
			bbox = AABB(bbox, objects[objectIndex]->boundingBox());

		int axis = bbox.longestAxis();

		auto comparator = (axis == 0) ? boxXCompare
						: (axis == 1) ? boxYCompare
									  : boxZCompare;

		size_t objectSpan = end - start;

		if (objectSpan == 1) {
			left = right = objects[start];
		}
		else if (objectSpan == 2) {
			left = objects[start];
			right = objects[start + 1];
		}
		else {
			std::sort(std::begin(objects) + start, std::begin(objects) + end, comparator);

			auto mid = start + objectSpan / 2;
			left = std::make_shared<BVHNode>(objects, start, mid);
			right = std::make_shared<BVHNode>(objects, mid, end);
		}
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

	static bool boxCompare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b, int axisIndex) {
		auto aAxisInterval = a->boundingBox().axisInterval(axisIndex);
		auto bAxisInterval = b->boundingBox().axisInterval(axisIndex);
		return aAxisInterval.min < bAxisInterval.min;
	}

	static bool boxXCompare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b) {
		return boxCompare(a, b, 0);
	}

	static bool boxYCompare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b) {
		return boxCompare(a, b, 1);
	}

	static bool boxZCompare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b) {
		return boxCompare(a, b, 2);
	}
};