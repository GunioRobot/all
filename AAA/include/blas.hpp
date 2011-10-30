#pragma once

#include <std/math>
#include <std/types>

#include "blas/Vector3.hpp"
#include "blas/Matrix3.hpp"
#include "blas/Matrix4.hpp"
#include "blas/Quaternion.hpp"

// Builders
inline Matrix4 perspective(float fovy, float aspect, float n, float f) {
	const float t = n * tanf(radians(fovy / 2));
	const float r = t * aspect;

	const float d = f - n;

	return Matrix4(
		n / r, 0, 0, 0,
		0, n / t, 0, 0,
		0, 0, -(f + n) / d, -1,
		0, 0, -(f * n * 2) / d, 0
	);
}

inline Matrix4 orthographic(float l, float r, float b, float t, float n, float f) {
	const float d = f - n;

	return Matrix4(
		2 / (r - l), 0, 0, 0,
		0, 2 / (t - b), 0, 0,
		0, 0, -2 / d, 0,
		-((r + l) / (r - l)), -((t + b) / (t - b)), -((f + n) / d), 1
	);
}

inline Quaternion rotation(const Vector3& euler_angles) {
	const Vector3 half = euler_angles * 0.5f;

	const float sx = sinf(half.x);
	const float sy = sinf(half.y);
	const float sz = sinf(half.z);

	const float cx = cosf(half.x);
	const float cy = cosf(half.y);
	const float cz = cosf(half.z);

	const float cycz = cy * cz;
	const float sysz = sy * sz;
	const float sycz = sy * cz;
	const float cysz = cy * sz;

	const float w = (cx * cycz) + (sx * sysz);
	const float x = (sx * cycz) - (cx * sysz);
	const float y = (cx * sycz) + (sx * cysz);
	const float z = (cx * cysz) - (sx * sycz);

	return Quaternion(w, x, y, z);
}
inline Quaternion rotation(const Vector3& axis, const float angle) {
	const float half_angle = angle * 0.5f;
	const float sv = sinf(half_angle);

	const float w = cosf(half_angle);
	const float x = axis.x * sv;
	const float y = axis.y * sv;
	const float z = axis.z * sv;

	return Quaternion(w, x, y, z);
}

// Conversions
inline Matrix3 toMatrix3(const Quaternion& a) {
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
inline Matrix4 toMatrix4(const Matrix3& a) {
	return Matrix4(
		a[0], a[1], a[2], 0,
		a[3], a[4], a[5], 0,
		a[6], a[7], a[8], 0,
		0, 0, 0, 1
	);
}
inline Matrix4 toMatrix4(const Quaternion& a) { return toMatrix4(toMatrix3(a)); }

// Transforms
inline void scale(Matrix4& m, const Vector3& s) {
	Matrix4 sm(
		s.x, 0, 0, 0,
		0, s.y, 0, 0,
		0, 0, s.z, 0,
		0, 0, 0, 1
	);
	m *= sm;
}
inline void rotate(Matrix4& m, const Quaternion& q) { m *= getMatrix4(q); }
inline void translate(Matrix4& m, const Vector3& t) {
	Matrix4 tm(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		t.x, t.y, t.z, 1
	);
	m *= tm;
}
