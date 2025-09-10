#pragma once

#include "Global.h"

class Texture
{
public:
	virtual ~Texture() = default;

	virtual color value(float u, float v, const vec3& p) const = 0;
};

class SolidColor : public Texture
{
public:
	SolidColor(const color& albedo) : albedo(albedo) {}

	SolidColor(float red, float green, float blue) : SolidColor(color(red, green, blue)) {}

	color value(float u, float v, const vec3& p) const override {
		return albedo;
	}

private:
	color albedo;
};

class CheckerTexture : public Texture
{
public:
	CheckerTexture(float scale, std::shared_ptr<Texture> even, std::shared_ptr<Texture> odd)
		: invScale(1.0 / scale), even(even), odd(odd) {}

	CheckerTexture(float scale, const color& c1, const color& c2)
		: CheckerTexture(scale, std::make_shared<SolidColor>(c1), std::make_shared<SolidColor>(c2)) {}

	color value(float u, float v, const vec3& p) const override {
		auto xInteger = int(std::floor(invScale * p.x));
		auto yInteger = int(std::floor(invScale * p.y));
		auto zInteger = int(std::floor(invScale * p.z));
		
		bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;
		return isEven ? even->value(u, v, p) : odd->value(u, v, p);
	}

private:
	float invScale;
	std::shared_ptr<Texture> even;
	std::shared_ptr<Texture> odd;
};
