#pragma once

#include "Hittable.h"

class Material
{
public:
	virtual ~Material() = default;

	virtual bool Scatter(const Ray& rayIn, const HitPayload& payload, color& attenuation, Ray& scattered) const {
		return false;
	}
};

class Lambertian : public Material
{
public:
	Lambertian(const color& albedo) : albedo(albedo) {}

	bool Scatter(const Ray& rayIn, const HitPayload& payload, color& attenuation, Ray& scattered) const override {
		vec3 scatterDir = payload.normal + randomUnit();

		// Catch degenerate scatter direction
		if (scatterDir.nearZero())
			scatterDir = payload.normal;

		scattered = Ray(payload.p, scatterDir);
		attenuation = albedo;
		return true;
	}

private:
	color albedo;
};

class Metal : public Material
{
public:
	Metal(const color& albedo, float fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

	bool Scatter(const Ray& rayIn, const HitPayload& payload, color& attenuation, Ray& scattered) const override {
		vec3 reflected = reflect(rayIn.GetDirection(), payload.normal);
		reflected = reflected.normalized() + (fuzz * randomUnit());
		scattered = Ray(payload.p, reflected);
		attenuation = albedo;
		return dot(scattered.GetDirection(), payload.normal) > 0.0f;
	}

private:
	color albedo;
	float fuzz;
};
