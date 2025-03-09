#pragma once

#include "Global.h"	

struct vec3
{
	float x, y, z;

	vec3() : x(0.0f), y(0.0f), z(0.0f) {}
	vec3(float x) : x(x), y(x), z(x) {}
	vec3(float x, float y, float z) : x(x), y(y), z(z) {}

	float& operator[](int index) { return index == 0 ? x : (index == 1 ? y : z); }
	const float& operator[](int index) const { return index == 0 ? x : (index == 1 ? y : z); }

	vec3 operator+(const vec3& other) const { return vec3(x + other.x, y + other.y, z + other.z); }
	vec3 operator-(const vec3& other) const { return vec3(x - other.x, y - other.y, z - other.z); }
	vec3 operator-() const { return vec3(-x, -y, -z); }
	vec3 operator*(const vec3& other) const { return vec3(x * other.x, y * other.y, z * other.z); }
	vec3 operator*(float k) const { return vec3(k * x, k * y, k * z); } // ÓÒ³Ë
	friend inline vec3 operator*(float k, const vec3& v); // ×ó³Ë
	vec3 operator/(float k) const { return *this * (1.0f / k); }

	vec3& operator+=(const vec3& other) { 
		*this = *this + other;
		return *this;
	}
	vec3& operator-=(const vec3& other) {
		*this = *this - other;
		return *this;
	}
	vec3& operator*=(float k) {
		*this = *this * k;
		return *this;
	}
	vec3& operator/=(float k) {
		*this = *this / k;
		return *this;
	}

	float norm() const { return sqrt(x * x + y * y + z * z); }
	float norm2() const { return x * x + y * y + z * z; }
	vec3 normalized() const
	{
		float length = norm();
		return vec3(x / length, y / length, z / length);
	}

	bool nearZero() const {
		auto s = 1e-8;
		return (std::fabs(x) < s) && (std::fabs(y) < s) && (std::fabs(z) < s);
	}

	static vec3 Random() { return vec3(randomFloat(), randomFloat(), randomFloat()); }
	static vec3 Random(float min, float max) { return vec3(randomFloat(min, max), randomFloat(min, max), randomFloat(min, max)); }

};

inline vec3 operator*(float k, const vec3& v) { return vec3(k * v.x, k * v.y, k * v.z); }
inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
	return out << v.x << " " << v.y << " " << v.z;
}

inline float dot(const vec3& a, const vec3& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
inline vec3 cross(const vec3& a, const vec3& b) { return vec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); }

inline vec3 randomUnit() {
	while (true) {
		vec3 p = vec3::Random(-1.0f, 1.0f);
		float lensq = p.norm2();
		if (1e-160 < lensq && lensq <= 1)
			return p / sqrtf(lensq);
	}
}

inline vec3 randomInUnitDisk() {
	while (true) {
		vec3 p = vec3(randomFloat(-1.0f, 1.0f), randomFloat(-1.0f, 1.0f), 0.0f);
		if (p.norm() < 1)
			return p;
	}
}

inline vec3 randomOnHemisphere(const vec3& normal) {
	vec3 onUnitSphere = randomUnit();
	if (dot(onUnitSphere, normal) > 0.0f) // In the same hemisphere as the normal
		return onUnitSphere;
	return -onUnitSphere;
}

inline vec3 reflect(const vec3& I, const vec3& N) {
	return I - 2.0f * dot(I, N) * N;
}

inline vec3 refract(const vec3& I, const vec3& N, float EtaiOverEtat) {
	float cosi = std::fmax(-1.0f, std::fmin(1.0f, dot(-I, N)));
	vec3 rOutPerp = EtaiOverEtat * (I + cosi * N);
	vec3 rOutParallel = -std::sqrtf(1 - EtaiOverEtat * EtaiOverEtat * (1 - cosi * cosi)) * N;
	return rOutPerp + rOutParallel;
}