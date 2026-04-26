#pragma once

#include "Hittable.h"
#include "HittableList.h"

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

		// Determine if the hit point lies within the planar shape using its plane coordinates.
		auto intersection = ray.at(t);
		vec3 planarHitptVector = intersection - Q;
		float alpha = dot(w, cross(planarHitptVector, v));
		float beta = dot(w, cross(u, planarHitptVector));

		if (!isInterior(alpha, beta, payload))
			return false;

		// Ray hits the 2D shape; set the rest of the hit record and return true.
		payload.t = t;
		payload.p = intersection;
		payload.material = mat;
		payload.SetFaceNormal(ray, normal);

		return true;
	}

	virtual bool isInterior(float a, float b, HitPayload& payload) const {
		Interval unitInterval = Interval(0, 1);
		// Given the hit point in plane coordinates, return false if it is outside the
        // primitive, otherwise set the hit record UV coordinates and return true.

		if (!unitInterval.contains(a) || !unitInterval.contains(b))
			return false;
		
		payload.u = a;
		payload.v = b;
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

inline std::shared_ptr<HittableList> box(const vec3& a, const vec3& b, std::shared_ptr<Material> mat) {
	// Returns the 3D box (six sides) that contains the two opposite vertices a & b.
	auto sides = std::make_shared<HittableList>();

	// Construct the two opposite vertices with the minimum and maximum coordinates.
	auto min = vec3(std::fmin(a.x, b.x), std::fmin(a.y, b.y), std::fmin(a.z, b.z));
	auto max = vec3(std::fmax(a.x, b.x), std::fmax(a.y, b.y), std::fmax(a.z, b.z));

	auto dx = vec3(max.x - min.x, 0, 0);
	auto dy = vec3(0, max.y - min.y, 0);
	auto dz = vec3(0, 0, max.z - min.z);

	sides->add(std::make_shared<Quad>(vec3(min.x, min.y, max.z), dx, dy, mat));  // front
	sides->add(std::make_shared<Quad>(vec3(max.x, min.y, max.z), -dz, dy, mat)); // right
	sides->add(std::make_shared<Quad>(vec3(max.x, min.y, min.z), -dx, dy, mat)); // back
	sides->add(std::make_shared<Quad>(vec3(min.x, min.y, min.z), dz, dy, mat));  // left
	sides->add(std::make_shared<Quad>(vec3(min.x, max.y, max.z), dx, -dz, mat)); // top
	sides->add(std::make_shared<Quad>(vec3(min.x, min.y, min.z), dx, dz, mat));  // bottom

	return sides;
}
