#include "std/assert.h"
#include "std/types.h"

#include "ogl/texture.h"

uint Texture::s_id = 0;

Texture::Texture(const Image& texture) {
	glGenTextures(1, &id);
	bind();

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, 3, texture.width, texture.height, 0, GL_BGR, GL_UNSIGNED_BYTE, texture.data);
	GL_CHECK_ERRORS
}
Texture::~Texture() {
	glDeleteTextures(1, &id);
	GL_CHECK_ERRORS
}
