#pragma once

#include "Global.h"

class Texture
{
public:
	virtual ~Texture() = default;

	virtual vec3 value(float u, float v, const vec3& p) const = 0;
};

class SolidColor : public Texture
{
public:
	SolidColor(const vec3& albedo) : albedo(albedo) {}

	SolidColor(float red, float green, float blue) : SolidColor(vec3(red, green, blue)) {}

	vec3 value(float u, float v, const vec3& p) const override {
		return albedo;
	}

private:
	vec3 albedo;
};
