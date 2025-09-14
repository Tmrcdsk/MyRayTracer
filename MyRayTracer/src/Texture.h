#pragma once

#include "Global.h"

#include "RtwImage.h"

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

class ImageTexture : public Texture
{
public:
	ImageTexture(const char* filename) : image(filename) {}

	color value(float u, float v, const vec3& p) const override {
		// If we have no texture data, then return solid cyan as a debugging aid.
		if (image.height() <= 0) return color(0, 1, 1);

		// Clamp input texture coordinates to [0,1] x [1,0]
		u = Interval(0, 1).clamp(u);
		v = 1.0f - Interval(0, 1).clamp(v); // Flip V to image coordinates

		auto i = int(u * image.width());
		auto j = int(v * image.height());
		auto pixel = image.pixelData(i, j);

		auto colorScale = 1.0f / 255.0f;
		return color(colorScale * pixel[0], colorScale * pixel[1], colorScale * pixel[2]);
	}

private:
	RtwImage image;
};
