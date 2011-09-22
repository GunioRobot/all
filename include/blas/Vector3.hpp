#pragma once

#include <std/math>

struct Vector3;
inline Vector3	add(const Vector3& a, const Vector3& b);
inline float	distance2(const Vector3& a, const Vector3& b);
inline Vector3	inverse(const Vector3& a);
inline float	length2(const Vector3& a);
inline Vector3	multiply(const Vector3& a, const Vector3& b);
inline Vector3	subtract(const Vector3& a, const Vector3& b);

struct Vector3 {
	float x, y, z;

	Vector3() {}
	Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

	inline Vector3 operator-() const { return inverse(*this); }
	inline Vector3 operator+(const Vector3& b) const { return add(*this, b); }
	inline Vector3 operator-(const Vector3& b) const { return subtract(*this, b); }
	inline Vector3 operator*(const Vector3& b) const { return multiply(*this, b); }
	inline Vector3& operator+=(const Vector3& b) { *this = *this + b; return *this; }
	inline Vector3& operator-=(const Vector3& b) { *this = *this - b; return *this; }
	inline Vector3& operator*=(const Vector3& b) { *this = *this * b; return *this; }

	inline Vector3 operator+(float f) const { return (*this + Vector3(f, f, f)); }
	inline Vector3 operator-(float f) const { return (*this - Vector3(f, f, f)); }
	inline Vector3 operator*(float f) const { return (*this * Vector3(f, f, f)); }
	inline Vector3& operator+=(float f) { *this = *this + f; return *this; }
	inline Vector3& operator-=(float f) { *this = *this - f; return *this; }
	inline Vector3& operator*=(float f) { *this = *this * f; return *this; }

	inline bool operator==(const Vector3& b) { return (x == b.x) && (y == b.y) && (z == b.z); }
	inline bool operator!=(const Vector3& b) { return !(*this == b); }
};

inline Vector3	add(const Vector3& a, const Vector3& b)		{ return Vector3(a.x + b.x, a.y + b.y, a.z + b.z); }
inline Vector3	cross(const Vector3& a, const Vector3& b)	{ return Vector3((a.y * b.z) - (a.z * b.y), (a.z * b.x) - (a.x * b.z), (a.x * b.y) - (a.y * b.x)); }
inline float	distance(const Vector3& a, const Vector3& b)	{ return sqrtf(distance2(a, b)); }
inline float	distance2(const Vector3& a, const Vector3& b)	{ return length2(a - b); }
inline float	dot(const Vector3& a, const Vector3& b)		{ return ((a.x * b.x) + (a.y * b.y) + (a.z * b.z)); }
inline Vector3	inverse(const Vector3& a)			{ return Vector3(-a.x, -a.y, -a.z); }
inline float	length(const Vector3& a)			{ return sqrtf(length2(a)); }
inline float	length2(const Vector3& a)			{ return dot(a, a); }
inline Vector3	multiply(const Vector3& a, const Vector3& b)	{ return Vector3(a.x * b.x, a.y * b.y, a.z * b.z); }
inline Vector3	normal(const Vector3& a)			{ const float il = invsqrtf(length2(a)); return (a * il); }
inline Vector3	subtract(const Vector3& a, const Vector3& b)	{ return Vector3(a.x - b.x, a.y - b.y, a.z - b.z); }
