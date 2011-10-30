#include <cstdio>

// notes:
// does not work in the vertical line case
// possibly not hte horizontal line
// works in all others
//
// line segments not done... to check just plug in the values of the intersect into either equation
// and check if equal


/*bool intersect(const vec2& p0, const vec2& p1, const vec2& p2, const vec2& p3, vec2* i) {
	float s1.x, s1.y, s2.x, s2.y;
	s1.x = p1.x - p0.x;
	s1.y = p1.y - p0.y;
	s2.x = p3.x - p2.x;
	s2.y = p3.y - p2.y;

	float s, t;
	s = (-s1.y * (p0.x - p2.x) + s1.x * (p0.y - p2.y)) / (-s2.x * s1.y + s1.x * s2.y);
	t = ( s2.x * (p0.y - p2.y) - s2.y * (p0.x - p2.x)) / (-s2.x * s1.y + s1.x * s2.y);

	if (s >= 0 && s <= 1 && t >= 0 && t <= 1) {
		// Collision detected
		if (i.x != NULL) *i.x = p0.x + (t * s1.x);
		if (i.y != NULL) *i.y = p0.y + (t * s1.y);

		return 1;
	}

	return 0; // No collision
}*/

struct vec2 {
	float x, y;

	vec2() {}
	vec2(float x, float y) : x(x), y(y) {}
};

float gradient(vec2 p1, vec2 p2) { return (p2.y - p1.y) / (p2.x - p1.x); }
float getYint(vec2 p, float m) { return (p.y - m * p.x); }
float getXint(vec2 p, float m) { return -(p.y - m * p.x) / m; }

vec2 intersect(vec2 p1, float m1, vec2 p2, float m2) {
	float y1 = getYint(p1, m1);
	float y2 = getYint(p2, m2);

	vec2 i; // intersect
	i.x = (y2 - y1) / (m1 - m2);
	i.y = m1 * i.x + y1;
	return i;
}

struct segment {
	vec2 p1, p2;
	segment(vec2 a, vec2 b) : p1(a), p2(b) {}
};

// checks if point is between two line segments
vec2 segment_intersect(segment, segment) {
	return vec2(0,0);
}

int main() {
	segment s1 = segment(vec2(0, 0), vec2(3, 3));
	segment s2 = segment(vec2(1, 0), vec2(4, 1));
	vec2 ret;// = intersect(s1, s2);
	//std::cout << "Intersection @ (" << ret.x << ", " << ret.y << ")\n" << std::endl;

	vec2 p1 = vec2(0,0);
	vec2 p2 = vec2(0,0);
	float m1 = gradient(p1, vec2(0,1));
	float m2 = gradient(p2, vec2(1,1));

	printf("m1: %f\n", m1);
	printf("m2: %f\n", m2);

	ret = intersect(p1, m1, p2, m2);
	printf("intersect @ %f, %f\n", ret.x, ret.y);

	return 0;
}

