#include <cstdio>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "blas.hpp"
#include "util/scope_timer.hpp"

#include <std/io>

#include <iostream>

struct Camera {
	Vector3 m_pos;
	float m_heading, m_pitch;

	Matrix4 m_view;

	Camera(Vector3 origin, float heading = 0, float pitch = 0) : m_pos(origin), m_heading(heading), m_pitch(pitch) { update(); }

	void update() {
		const Quaternion q = rotation(Vector3(m_pitch, m_heading, 0.f));

		m_view = getMatrix4(q);
		translate(m_view, m_pos);
	}
};

void printmat(const glm::mat4& p) {
	for (int i = 0; i < 16; i += 4) printf("%f %f %f %f\n", (&p[0][0])[i], (&p[0][0])[i + 1], (&p[0][0])[i + 2], (&p[0][0])[i + 3]);
}
void printmat(const Matrix4& p) {
	for (int i = 0; i < 16; i += 4) printf("%f %f %f %f\n", p[i], p[i+1], p[i+2], p[i+3]);
}

void assertcomp(const Matrix4& p, const glm::mat4& q) {
	for (int i = 0; i < 16; ++i) assert(fabs((&q[0][0])[i] - p[i]) < 0.0001); // GLM's floating point sucks ass
}

int main() {
	float fovy = 65.f, w = 800.f, h = 600.f, near = 0.1f, far = 100.f;
	Vector3 m_pos(0, 0, 20);
	float m_heading = 5, m_pitch = 20;

	Quaternion ar = rotation(Vector3(m_pitch, m_heading, 0.f));
	Matrix4 av =  getMatrix4(ar);
	relative_translate(av, m_pos);

	Matrix4 a1 = perspective(fovy, w / h, near, far);
	Matrix4 a2(1);
	a2[12] = 5;//x
	a2[13] = 4;//y
	a2[14] = 5;//z
	Matrix4 a3 = a1 * a2; // pre-multiply, row major, a1 transformed by a2
	Matrix4 a4 = a3 * av;
	a4 = a1 * a4; // pre-multiply, row major

	glm::quat gr(glm::vec3(m_pitch, m_heading, 0.f));
	glm::mat4 gv = glm::mat4_cast(gr);
	gv = glm::translate(gv, glm::vec3(m_pos.x, m_pos.y, m_pos.z));

	glm::mat4 g1 = glm::perspective(fovy, w / h, near, far);
	glm::mat4 g2(1);
	g2[3][0] = 5;//x
	g2[3][1] = 4;//y
	g2[3][2] = 5;//z
	glm::mat4 g3 = g2 * g1; // post-multiply, column major, g2 transforms g1
	glm::mat4 g4 = gv * g3;
	g4 = g4 * g1; // post-multiply, column major

	printf("\nROM\n");
	printf("Original :\n"); printmat(a1);
	printf("Translation :\n"); printmat(a2);
	printf("Rotation :\n"); printmat(av);
	printf("Result :\n"); printmat(a3);
	printf("Mult :\n"); printmat(a4);
	printf("\nGLM\n");
	printf("Original :\n"); printmat(g1);
	printf("Translation :\n"); printmat(g2);
	printf("Rotation :\n"); printmat(gv);
	printf("Result :\n"); printmat(g3);
	printf("Mult :\n"); printmat(g4);

	// assert compare the matrices, they should be equal!
	assertcomp(a1, g1);
	assertcomp(a2, g2);
	assertcomp(a3, g3);
	assertcomp(a4, g4);

	printf("\nUnit Tests completed\n\n");

	const int iterations = 1e7;
	{
		ScopeTimer t("ROM Matrix Mult");
		for (int i = 0; i < iterations; ++i) {
			a1 = av * a2;
			a2 = a1 * a1;
			a4 = a2 * a3;
			relative_translate(a3, Vector3(1,34,2));
		}
		printmat(a3);
	}
	{
		ScopeTimer t("GLM Matrix Mult");
		for (int i = 0; i < iterations; ++i) {
			g1 = g2 * gv;
			g2 = g1 * g1;
			g4 = g3 * g2;
			g3 = glm::translate(g3, glm::vec3(1, 34, 2));
		}
		printmat(g3);
	}
	return 0;
}

