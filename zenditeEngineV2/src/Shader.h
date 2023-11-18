#pragma once
#include "utils.h"

class Shader
{
private:
	unsigned int shaderProgHandle;
	unsigned int vShader;
	unsigned int fShader;
	const char* vs_string;
	const char* fs_string;

	void createAndCompileVertShader();
	void createAndCompileFragShader();
	void linkShaderPrograms();


public:
	Shader(const char* vs, const char* fs);

	unsigned int getShaderHandle();

};