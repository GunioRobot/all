#pragma once

#include <std/assert>
#include "blas.hpp"

class BoundingBox {
	Vector3 m_bounds;
public:
	BoundingBox(const float* points, size_t n_points) {
		Vector3 min(FLT_INF, FLT_INF, FLT_INF), max(-FLT_INF,-FLT_INF,-FLT_INF);

		assert(points != 0);
		assert(n_points > 0);
		assert(n_points % 3 == 0);

		for (size_t i = 0; i < n_points; i += 3) {
			const float x = points[i];
			const float y = points[i + 1];
			const float z = points[i + 2];

			if (x > max.x) max.x = x;
			if (y > max.y) max.y = y;
			if (z > max.z) max.z = z;

			if (x < min.x) min.x = x;
			if (y < min.y) min.y = y;
			if (z < min.z) min.z = z;
		}

		assert(min.x != FLT_INF);
		assert(min.y != FLT_INF);
		assert(min.z != FLT_INF);
		assert(max.x != -FLT_INF);
		assert(max.y != -FLT_INF);
		assert(max.z != -FLT_INF);

		m_bounds = max - min;
	}

	bool overlaps(const BoundingBox& b, const Vector3& a_origin, const Vector3& b_origin) const {
		const Vector3 delta = a_origin - b_origin;
		return	fabs(delta.x) <= (m_bounds.x + b.m_bounds.x) &&
			fabs(delta.y) <= (m_bounds.y + b.m_bounds.y) &&
			fabs(delta.z) <= (m_bounds.z + b.m_bounds.z);
	}
};

class BoundingSphere {
	float m_radius;
public:
	BoundingSphere(const float* points, size_t n_points) {
		float min = FLT_INF, max = -FLT_INF;

		assert(points != 0);
		assert(n_points > 0);
		assert(n_points % 3 == 0);

		for (size_t i = 0; i < n_points; i += 3) {
			const float x = points[i];
			const float y = points[i + 1];
			const float z = points[i + 2];

			if (x > max) max = x;
			if (y > max) max = y;
			if (z > max) max = z;

			if (x < min) min = x;
			if (y < min) min = y;
			if (z < min) min = z;
		}

		assert(min != FLT_INF);
		assert(max != -FLT_INF);

		m_radius = (max - min) / 2.f;
	}

	bool overlaps(const BoundingSphere& b, const Vector3& a_origin, const Vector3& b_origin) const {
		const float distance2 = a_origin.distance2(b_origin);
		const float sum = m_radius + b.m_radius;
		return distance2 <= (sum * sum);
	}
};
