#pragma once
#include "utils.h"

#include <string>
#include <fstream>
#include <sstream>

class Shader
{
private:
	unsigned int shaderProgHandle;
	unsigned int vShader;
	unsigned int fShader;
	std::string vs_string;
	std::string fs_string;

	void createAndCompileVertShader();
	void createAndCompileFragShader();
	void linkShaderPrograms();


public:
	Shader(const char* vs_path, const char* fs_path);

	unsigned int getShaderHandle() const;
	void readShaderCodeFromFile(const char* vs_path, const char* fs_path);

	void setUniform4f(std::string name, float v1, float v2, float v3, float v4) const;
	void setUniform3fv(std::string name, glm::vec3 &vec) const;
	void setUniform3fv(std::string name, float v1, float v2, float v3) const;
	void setUniformTextureUnit(std::string name, unsigned int x);
	void setUniformMat4(std::string name, GLboolean transpose, const GLfloat* mat);

	void bindProgram() const;
};