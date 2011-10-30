#pragma once

struct Matrix3;
inline Matrix3 multiply(const Matrix3& a, const Matrix3& b);

struct Matrix3 {
	float m[9];

	Matrix3() {}
	Matrix3(float a) {
		m[0] = a; m[1] = 0; m[2] = 0;
		m[3] = 0; m[4] = a; m[5] = 0;
		m[6] = 0; m[7] = 0; m[8] = a;
	}
	Matrix3(float a1, float a2, float a3,
		float b1, float b2, float b3,
		float c1, float c2, float c3
	) {
		m[0] = a1; m[1] = a2; m[2] = a3;
		m[3] = b1; m[4] = b2; m[5] = b3;
		m[6] = c1; m[7] = c2; m[8] = c3;
	}

	inline Matrix3 operator*(const Matrix3& b) const { return multiply(*this, b); }
	inline Matrix3& operator*=(const Matrix3& b) { *this = b * *this; return *this; }

	inline float& operator[](size_t i) { return m[i]; }
	inline const float& operator[](size_t i) const { return m[i]; }
};

inline Matrix3 multiply(const Matrix3& a, const Matrix3& b) {
	return Matrix3(
		a[0] * b[0] + a[1] * b[3] + a[2] * b[6],
		a[0] * b[1] + a[1] * b[4] + a[2] * b[7],
		a[0] * b[2] + a[1] * b[5] + a[2] * b[8],
		a[3] * b[0] + a[4] * b[3] + a[5] * b[6],
		a[3] * b[1] + a[4] * b[4] + a[5] * b[7],
		a[3] * b[2] + a[4] * b[5] + a[5] * b[8],
		a[6] * b[0] + a[7] * b[3] + a[8] * b[6],
		a[6] * b[1] + a[7] * b[4] + a[8] * b[7],
		a[6] * b[2] + a[7] * b[5] + a[8] * b[8]
	);
}
inline Matrix3 transpose(const Matrix3& a) {
	return Matrix3(
		a[0], a[3], a[6],
		a[1], a[4], a[7],
		a[2], a[5], a[8]
	);
}
