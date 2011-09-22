#pragma once

#include <std/math>
#include <std/types>

#include "blas/Vector3.hpp"
#include "blas/Matrix3.hpp"
#include "blas/Matrix4.hpp"
#include "blas/Quaternion.hpp"

// All declared inline for static linkage, and implied that most will be inlined anyway.
// Builders
inline Matrix4 perspective(float fovy, float aspect, float zNear, float zFar) {
	const float yMax = zNear * tanf(radians(fovy / 2));
	const float yMin = -yMax;

	const float xMax = yMax * aspect;
	const float xMin = yMin * aspect;

	const float zRange = zFar - zNear;
	const float zNear2 = 2 * zNear;

	return Matrix4(
		zNear2 / (xMax - xMin), 0, 0, 0,
		0, zNear2 / (yMax - yMin), 0, 0,
		0, 0, -(zFar + zNear) / zRange, -1,
		0, 0, -(zFar * zNear2) / zRange, 0
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
inline void translate(Matrix4& m, const Vector3& t) {
	m[12] = t.x;
	m[13] = t.y;
	m[14] = t.z;
}
inline void rotate(Matrix4& m, const Quaternion& q) { m *= getMatrix4(q); }
inline void relative_translate(Matrix4& m, const Vector3& t) {
	Matrix4 tm(1);
	translate(tm, t);
	m *= tm;
}
