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

class Dielectric : public Material
{
public:
	Dielectric(float refractIndex) : refractIndex(refractIndex) {}

	bool Scatter(const Ray& rayIn, const HitPayload& payload, color& attenuation, Ray& scattered) const override {
		attenuation = color(1.0f);
		float ri = payload.front_face ? (1.0f / refractIndex) : refractIndex;

		vec3 unitDir = rayIn.GetDirection().normalized();
		float cosi = std::fmax(-1.0f, std::fmin(1.0f, dot(-unitDir, payload.normal)));
		float sini = std::sqrtf(1.0f - cosi * cosi);

		bool cannotRefract = ri * sini > 1.0f;
		vec3 dir;
		if (cannotRefract || reflectance(cosi, ri) > randomFloat())
			dir = reflect(unitDir, payload.normal);
		else
			dir = refract(unitDir, payload.normal, ri);

		scattered = Ray(payload.p, dir);
		return true;
	}

private:
	static float reflectance(float cosi, float refractIndex) {
		// Use Schlick's approximation for reflectance.
		float r0 = (1 - refractIndex) / (1 + refractIndex);
		r0 *= r0;
		return r0 + (1 - r0) * std::powf(1.0f - cosi, 5.0f);
	}

private:
	float refractIndex;
};
