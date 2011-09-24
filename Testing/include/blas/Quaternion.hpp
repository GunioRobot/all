#pragma once

#include <std/math>

#include "Vector3.hpp"
#include "Matrix3.hpp"
#include "Matrix4.hpp"

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

inline Matrix3 getMatrix3(const Quaternion& a) {
	const float s = 2.f / length2(a);

	const float xs = a.x * s, ys = a.y * s, zs = a.z * s;
	const float wx = a.w * xs, wy = a.w * ys, wz = a.w * zs;
	const float xx = a.x * xs, xy = a.x * ys, xz = a.x * zs;
	const float yy = a.y * ys, yz = a.y * zs, zz = a.z * zs;

	return Matrix3(
		1.f - (yy + zz),	xy + wz,		xz - wy,
		xy - wz,		1.f - (xx + zz),	yz + wx,
		xz + wy,		yz - wx,		1.f - (xx + yy)
	);
}
inline Matrix4 getMatrix4(const Quaternion& a) { return Matrix4(getMatrix3(a)); }
