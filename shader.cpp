#include"shader.h"

std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

shader::shader(const char* vertexFile, const char* fragmentFile)
{
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);

	glCompileShader(vertexShader);

	int success;
	char infolog[512];
	
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infolog);
		std::cout << "ERROR::VERTEX::COMPILATION::FAILED\n" << infolog << std::endl;
	}


	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infolog);
		std::cout << "ERROR::FRAGMENT::COMPILATION::FAILED\n" << infolog << std::endl;
	}


	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}

void shader::Activate()
{
	glUseProgram(ID);
}

void shader::Delete()
{
	glDeleteProgram(ID);
}

void shader::setuniform4fm(const char* uniformName, glm::mat4 matrix)
{
	int uniformLoc = glGetUniformLocation(ID, uniformName);
	glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(matrix));
}

void shader::setuniform3v(const char* uniformName, glm::vec3 vector)
{
	int uniformLoc = glGetUniformLocation(ID, uniformName);
	glUniform3fv(uniformLoc, 1, glm::value_ptr(vector));
}

void shader::setuniform1i(const char* uniformName, int value)
{	
	unsigned int uniformLoc = glGetUniformLocation(ID, uniformName);
	glUniform1i(uniformLoc, value);
}

void shader::setuniform3fm(const char* uniformName, glm::mat3 matrix)
{
	unsigned int uniformLoc = glGetUniformLocation(ID, uniformName);
	glUniformMatrix3fv(ID, 1, GL_FALSE, glm::value_ptr(matrix));
}


