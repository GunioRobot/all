#include "std/assert.h"
#include "std/io.h"
#include "std/types.h"

#include "ogl/vao.h"

uint VAO::s_id = 0;

VAO::VAO(const Mesh& mesh) {
	const size_t n_attribs = 3;
	const VertexAttribute attribs[n_attribs] = {
		{0, {GL_FLOAT, 3}, false, {&mesh.points[0], mesh.sizeof_points()}}, // points
		{1, {GL_FLOAT, 3}, false, {&mesh.normals[0], mesh.sizeof_normals()}}, // normals
		{2, {GL_FLOAT, 2}, false, {&mesh.uvcoords[0], mesh.sizeof_uvcoords()}} // uvcoords
	};
	const size_t isize = mesh.sizeof_indices();
	const size_t vsize = mesh.sizeof_vertices();

	icount = mesh.n_indices();
	assert(icount > 0);

	glGenVertexArrays(1, &id);
	glBindVertexArray(id);
	GL_CHECK_ERRORS

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, isize, &mesh.indices[0], GL_STATIC_DRAW);
	GL_CHECK_ERRORS

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vsize, 0, GL_STATIC_DRAW);
	GL_CHECK_ERRORS

	size_t voffset = 0;
	for (size_t i = 0; i < n_attribs; ++i) {
		const VertexAttribute& va = attribs[i];

		glBufferSubData(GL_ARRAY_BUFFER, voffset, va.data.size, va.data.ptr);
		GL_CHECK_ERRORS

		glVertexAttribPointer(va.index, (int) va.element.count, va.element.type, va.normalised, 0, (void*) voffset);
		GL_CHECK_ERRORS

		glEnableVertexAttribArray(va.index);
		GL_CHECK_ERRORS

		voffset += va.data.size;
	}
}

