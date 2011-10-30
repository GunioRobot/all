#define GL_CHECK_ERRORS assert(glGetError() == GL_NO_ERROR);

GLSLShader colorshader, texshader;
void InitShaders() {
	colorshader.LoadFromFile(GL_VERTEX_SHADER, "shader.vert");
	colorshader.LoadFromFile(GL_FRAGMENT_SHADER, "shader.frag");
	colorshader.CreateAndLinkProgram();
	colorshader.Use();
		colorshader.AddAttribute("vVertex");
		colorshader.AddUniform("MVP");
	colorshader.UnUse();

	GL_CHECK_ERRORS

	texshader.LoadFromFile(GL_VERTEX_SHADER, "tex_shader.vert");
	texshader.LoadFromFile(GL_FRAGMENT_SHADER, "tex_shader.frag");
	texshader.CreateAndLinkProgram();
	texshader.Use();
		texshader.AddAttribute("vVertex");
		texshader.AddAttribute("vUV");
		texshader.AddUniform("MVP");
		texshader.AddUniform("textureMap");
		glUniform1i(texshader("textureMap"), 0);
	texshader.UnUse();

	GL_CHECK_ERRORS
}

uint vboVerticesID, vboUVID, vboIndicesID, vaoID;
void InitVAOs() {
	glGenVertexArrays(1, &vaoID);
	glGenBuffers(1, &vboVerticesID);
	glGenBuffers(1, &vboUVID);
	glGenBuffers(1, &vboIndicesID);
	GL_CHECK_ERRORS

	glBindVertexArray(vaoID);
		glBindBuffer (GL_ARRAY_BUFFER, vboVerticesID);
		glBufferData (GL_ARRAY_BUFFER, sizeof(cube.vertex), &cube.vertex[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(colorshader["vVertex"]);
		glVertexAttribPointer (colorshader["vVertex"], 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*3 ,0);
		GL_CHECK_ERRORS

		glBindBuffer (GL_ARRAY_BUFFER, vboUVID);
		glBufferData (GL_ARRAY_BUFFER, sizeof(cube.mapcoord), &cube.mapcoord[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(texshader["vUV"]);
		glVertexAttribPointer (texshader["vUV"], 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*2 ,0);
		GL_CHECK_ERRORS

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndicesID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube.polygons), &cube.polygons[0], GL_STATIC_DRAW);
		GL_CHECK_ERRORS
	glBindVertexArray(0);

	GL_CHECK_ERRORS
}
