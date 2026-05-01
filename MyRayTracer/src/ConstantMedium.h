#pragma once

#include "Hittable.h"
#include "Material.h"
#include "Texture.h"

class ConstantMedium : public Hittable
{
public:
    ConstantMedium(std::shared_ptr<Hittable> boundary, float density, std::shared_ptr<Texture> tex)
        : boundary(boundary), negInvDensity(-1 / density), phaseFunction(std::make_shared<Isotropic>(tex)) {}

    ConstantMedium(std::shared_ptr<Hittable> boundary, float density, const color& albedo)
        : boundary(boundary), negInvDensity(-1 / density), phaseFunction(std::make_shared<Isotropic>(albedo)) {}

    bool hit(const Ray& ray, Interval t, HitPayload& payload) const override {
        HitPayload payload1, payload2;

        if (!boundary->hit(ray, Interval::universe, payload1))
            return false;
        
        if (!boundary->hit(ray, Interval(payload1.t + 1e-4f, infinity), payload2))
            return false;
        
        if (payload1.t < t.min) payload1.t = t.min;
        if (payload2.t > t.max) payload2.t = t.max;

        if (payload1.t >= payload2.t)
            return false;
        
        if (payload1.t < 0)
            payload1.t = 0;
        
        auto rayLength = ray.GetDirection().norm();
        auto distanceInsideBoundary = (payload2.t - payload1.t) * rayLength;
        auto hitDistance = negInvDensity * std::log(randomFloat());

        if (hitDistance > distanceInsideBoundary)
            return false;
        
        payload.t = payload1.t + hitDistance / rayLength;
        payload.p = ray.at(payload.t);

        payload.normal = vec3(1, 0, 0); // arbitrary
        payload.front_face = true;      // also arbitrary
        payload.material = phaseFunction;

        return true;
    }

    AABB boundingBox() const override { return boundary->boundingBox(); }

private:
    std::shared_ptr<Hittable> boundary;
    float negInvDensity;
    std::shared_ptr<Material> phaseFunction;
};
