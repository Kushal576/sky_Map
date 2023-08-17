#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H


#include<glad/glad.h>
#include<fstream>
#include<sstream>
#include<iostream>
#include<string>
#include<cerrno>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

std::string get_file_contents(const char* filename);

class shader
{
public:
	GLuint ID;
	shader(const char* vertexFile, const char* fragmentFile);
	void Activate();
	void Delete();
	void setuniform4fv(const char* uniformName, glm::mat4 matrix);
};


#endif