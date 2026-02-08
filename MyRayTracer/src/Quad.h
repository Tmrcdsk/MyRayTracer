#pragma once

#include "Hittable.h"

class Quad : public Hittable
{
public:
	Quad(const vec3& Q, const vec3& u, const vec3& v, std::shared_ptr<Material> mat)
		: Q(Q), u(u), v(v), mat(mat) {
		setBoundingBox();
	}

	virtual void setBoundingBox() {
		// Compute the bounding box of all four vertices.
		auto bbox_diagonal1 = AABB(Q, Q + u + v);
		auto bbox_diagonal2 = AABB(Q + u, Q + v);
		bbox = AABB(bbox_diagonal1, bbox_diagonal2);
	}

	AABB boundingBox() const override { return bbox; }

	bool hit(const Ray& ray, Interval t, HitPayload& payload) const override {
		return false; // To be implemented
	}

private:
	vec3 Q;
	vec3 u, v;
	std::shared_ptr<Material> mat;
	AABB bbox;
};
