#pragma once

#include <std/math>

struct Quaternion;
inline float length2(const Quaternion& a);
inline Quaternion multiply(const Quaternion& a, const Quaternion& b);

struct Quaternion {
	float w, x, y, z;

	Quaternion() {}
	Quaternion(float w, float x, float y, float z) : w(w), x(x), y(y), z(z) {}

	inline Quaternion operator*(const Quaternion& a) const { return multiply(*this, a); }
	inline Quaternion& operator*=(const Quaternion& a) { *this = *this * a; return *this; }
};

inline float dot(const Quaternion& a, const Quaternion& b) { return ((a.w * b.w) + (a.x * b.x) + (a.y * b.y) + (a.z * b.z)); }
inline float length(const Quaternion& a) { return sqrtf(length2(a)); }
inline float length2(const Quaternion& a) { return dot(a, a); }
inline Quaternion multiply(const Quaternion& a, const Quaternion& b) {
	return Quaternion(
		(a.w * b.w) - (a.x * b.x) - (a.y * b.y) - (a.z * b.z),
		(a.w * b.x) + (a.x * b.w) + (a.y * b.z) - (a.z * b.y),
		(a.w * b.y) - (a.x * b.z) + (a.y * b.w) + (a.z * b.x),
		(a.w * b.z) + (a.x * b.y) - (a.y * b.x) + (a.z * b.w)
	);
}
