#pragma once
#include <string>

struct Texture
{
	unsigned int texHandle;
	std::string type; //diffuse, specular, etc.
	std::string fullPath;
};