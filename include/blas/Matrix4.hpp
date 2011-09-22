#pragma once

#include "Matrix3.hpp"

struct Matrix4;
inline Matrix4 multiply(const Matrix4& a, const Matrix4& b);

struct Matrix4 {
	float m[16];

	Matrix4() {}
	Matrix4(float a) {
		m[0] = a; m[1] = 0; m[2] = 0; m[3] = 0;
		m[4] = 0; m[5] = a; m[6] = 0; m[7] = 0;
		m[8] = 0; m[9] = 0; m[10] = a; m[11] = 0;
		m[12] = 0; m[13] = 0; m[14] = 0; m[15] = a;
	}
	Matrix4(float a1, float a2, float a3, float a4,
		float b1, float b2, float b3, float b4,
		float c1, float c2, float c3, float c4,
		float d1, float d2, float d3, float d4
	) {
		m[0] = a1; m[1] = a2; m[2] = a3; m[3] = a4;
		m[4] = b1; m[5] = b2; m[6] = b3; m[7] = b4;
		m[8] = c1; m[9] = c2; m[10] = c3; m[11] = c4;
		m[12] = d1; m[13] = d2; m[14] = d3; m[15] = d4;
	}
	Matrix4(const Matrix3& b) {
		m[0] = b[0]; m[1] = b[1]; m[2] = b[2]; m[3] = 0;
		m[4] = b[3]; m[5] = b[4]; m[6] = b[5]; m[7] = 0;
		m[8] = b[6]; m[9] = b[7]; m[10] = b[8]; m[11] = 0;
		m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
	}

	inline Matrix4 operator*(const Matrix4& b) const { return multiply(*this, b); }
	inline Matrix4& operator*=(const Matrix4& b) { *this = b * *this; return *this; }

	inline float& operator[](size_t i) { return m[i]; }
	inline const float& operator[](size_t i) const { return m[i]; }
};

inline Matrix4 transpose(const Matrix4& a) {
	return Matrix4(
		a[0], a[4], a[8], a[12],
		a[1], a[5], a[9], a[13],
		a[2], a[6], a[10], a[14],
		a[3], a[7], a[11], a[15]
	);
}

inline Matrix4 multiply(const Matrix4& a, const Matrix4& b) {
	return Matrix4(
		a[0] * b[0] + a[1] * b[4] + a[2] * b[8] + a[3] * b[12],
		a[0] * b[1] + a[1] * b[5] + a[2] * b[9] + a[3] * b[13],
		a[0] * b[2] + a[1] * b[6] + a[2] * b[10] + a[3] * b[14],
		a[0] * b[3] + a[1] * b[7] + a[2] * b[11] + a[3] * b[15],
		a[4] * b[0] + a[5] * b[4] + a[6] * b[8] + a[7] * b[12],
		a[4] * b[1] + a[5] * b[5] + a[6] * b[9] + a[7] * b[13],
		a[4] * b[2] + a[5] * b[6] + a[6] * b[10] + a[7] * b[14],
		a[4] * b[3] + a[5] * b[7] + a[6] * b[11] + a[7] * b[15],
		a[8] * b[0] + a[9] * b[4] + a[10] * b[8] + a[11] * b[12],
		a[8] * b[1] + a[9] * b[5] + a[10] * b[9] + a[11] * b[13],
		a[8] * b[2] + a[9] * b[6] + a[10] * b[10] + a[11] * b[14],
		a[8] * b[3] + a[9] * b[7] + a[10] * b[11] + a[11] * b[15],
		a[12] * b[0] + a[13] * b[4] + a[14] * b[8] + a[15] * b[12],
		a[12] * b[1] + a[13] * b[5] + a[14] * b[9] + a[15] * b[13],
		a[12] * b[2] + a[13] * b[6] + a[14] * b[10] + a[15] * b[14],
		a[12] * b[3] + a[13] * b[7] + a[14] * b[11] + a[15] * b[15]
	);
}
