#include"VAO.h"

VAO::VAO()
{
	glGenVertexArrays(1, &ID);
}

void VAO::linkVBO(VBO &VBO,GLuint layout,GLuint number, GLuint size)
{
	VBO.Bind();
	glVertexAttribPointer(layout, number, GL_FLOAT, GL_FALSE, size * sizeof(float), (void*)(3*layout*sizeof(float)));
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
}

void VAO::linkEBO(EBO& EBO)
{
	EBO.Bind();
}

void VAO::Bind()
{
	glBindVertexArray(ID);
}

void VAO::Unbind()
{
	glBindVertexArray(0);
}

void VAO::Delete()
{
	glDeleteVertexArrays(1, &ID);
}