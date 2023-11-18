#include "Shader.h"

Shader::Shader(const char* vs, const char* fs)
{
	readShaderCodeFromFile(vs, fs);
	createAndCompileVertShader();
	createAndCompileFragShader();
	linkShaderPrograms();
}

unsigned int Shader::getShaderHandle() const
{
	return shaderProgHandle;
}


void Shader::readShaderCodeFromFile(const char* vs_path, const char* fs_path)
{
	vs_string;
	fs_string;

	std::ifstream vsFile;
	std::ifstream fsFile;

	vsFile.open(vs_path);
	fsFile.open(fs_path);

	if (!vsFile.is_open()) 
	{
		std::cerr << "\nError: Unable to open vertex shader file at: " << vs_path << std::endl;
		ASSERT(false);
	}
	if (!fsFile.is_open())
	{
		std::cerr << "\nError: Unable to open fragment shader file at: " << vs_path << std::endl;
		ASSERT(false);
	}

	std::stringstream vsStream;
	std::stringstream fsStream;

	vsStream << vsFile.rdbuf();
	fsStream << fsFile.rdbuf();

	vsFile.close();
	fsFile.close();

	vs_string = vsStream.str();
	fs_string = fsStream.str();
}

void Shader::setUniform4f(std::string name, float v1, float v2, float v3, float v4) const
{
	const char* n = name.c_str();
	GLCALL(glUniform4f(glGetUniformLocation(shaderProgHandle, n), v1, v2, v3, v4));
}

void Shader::setUniformTextureUnit(std::string name, int x)
{
	const char* n = name.c_str();
	unsigned int loc = glGetUniformLocation(shaderProgHandle, n);
	glUniform1i(loc, x);
}

void Shader::bindProgram() const
{
	GLCALL(glUseProgram(shaderProgHandle));
}

void Shader::createAndCompileVertShader()
{
	const char* vss = vs_string.c_str();

	vShader = glCreateShader(GL_VERTEX_SHADER);

	if (vShader == 0)
	{
		std::cout << "vShader could not be created" << std::endl;
		ASSERT(false);
	}
	
	GLCALL(glShaderSource(vShader, 1, &vss, NULL));
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
	const char* fss = fs_string.c_str();

	fShader = glCreateShader(GL_FRAGMENT_SHADER);

	if (fShader == 0)
	{
		std::cout << "fShader could not be created" << std::endl;
		ASSERT(false);
	}

	glShaderSource(fShader, 1, &fss, NULL);
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
