#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include<glad/glad.h>
#include"VBO.h"
#include"EBO.h"

class VAO
{
public:
	GLuint ID;
	VAO();
	void linkVBO(VBO& VBO, GLuint layout,GLuint number,GLuint size);
	void linkEBO(EBO& EBO);
	void Bind();
	void Unbind();
	void Delete();
};

#endif // !VAO_CLASS_H

