#pragma once

#include "Hittable.h"

class Quad : public Hittable
{
public:
	Quad(const vec3& Q, const vec3& u, const vec3& v, std::shared_ptr<Material> mat)
		: Q(Q), u(u), v(v), mat(mat) {
		auto n = cross(u, v);
		normal = n.normalized();
		D = dot(normal, Q);
		w = n / dot(n, n);

		setBoundingBox();
	}

	virtual void setBoundingBox() {
		// Compute the bounding box of all four vertices.
		auto bbox_diagonal1 = AABB(Q, Q + u + v);
		auto bbox_diagonal2 = AABB(Q + u, Q + v);
		bbox = AABB(bbox_diagonal1, bbox_diagonal2);
	}

	AABB boundingBox() const override { return bbox; }

	bool hit(const Ray& ray, Interval interval, HitPayload& payload) const override {
		auto denom = dot(normal, ray.GetDirection());

		// No hit if the ray is parallel to the plane.
		if (std::fabs(denom) < 1e-6f)
			return false;
		
		// Return false if the hit point parameter t is outside the ray interval.
		auto t = (D - dot(normal, ray.GetOrigin())) / denom;
		if (!interval.contains(t))
			return false;

		auto intersection = ray.at(t);

		payload.t = t;
		payload.p = intersection;
		payload.material = mat;
		payload.SetFaceNormal(ray, normal);

		return true;
	}

private:
	vec3 Q;
	vec3 u, v;
	vec3 w;
	std::shared_ptr<Material> mat;
	AABB bbox;
	vec3 normal;
	float D;
};
