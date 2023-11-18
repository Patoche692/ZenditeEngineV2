#include "Shader.h"

Shader::Shader(const char* vs, const char* fs): vs_string(vs), fs_string(fs)
{
	createAndCompileVertShader();
	createAndCompileFragShader();
	linkShaderPrograms();

}

unsigned int Shader::getShaderHandle()
{
	return shaderProgHandle;
}

void Shader::createAndCompileVertShader()
{
	vShader = glCreateShader(GL_VERTEX_SHADER);

	if (vShader == 0)
	{
		std::cout << "vShader could not be created" << std::endl;
		ASSERT(false);
	}

	GLCALL(glShaderSource(vShader, 1, &vs_string, NULL));
	GLCALL(glCompileShader(vShader));

	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}

void Shader::createAndCompileFragShader()
{
	fShader = glCreateShader(GL_FRAGMENT_SHADER);

	if (fShader == 0)
	{
		std::cout << "fShader could not be created" << std::endl;
		ASSERT(false);
	}

	glShaderSource(fShader, 1, &fs_string, NULL);
	glCompileShader(fShader);

	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}

void Shader::linkShaderPrograms()
{
	shaderProgHandle = glCreateProgram();
	GLCALL(glAttachShader(shaderProgHandle, vShader));
	GLCALL(glAttachShader(shaderProgHandle, fShader));
	GLCALL(glLinkProgram(shaderProgHandle));

	GLCALL(glDeleteShader(vShader));
	GLCALL(glDeleteShader(fShader));
}
