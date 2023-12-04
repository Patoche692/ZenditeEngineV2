#pragma once
#include "utils.h"

//Note: This class assumes all textures are 2D textures.

class Texture2D
{
private:
	unsigned int texHandle;
	int imgWidth;
	int imgHeight;
	int nrChannels;
	unsigned char* imgData;
	
	unsigned int texUnit;

public:
	Texture2D(); //Generate a texture handle
	void setupTextureJPG(unsigned int unit, const char* texfilepath);
	void setupTexturePNG(unsigned int unit, const char* texfilepath);
	void setTexParameteri(GLenum pname, GLint param) const;

	void changeTexUnit(unsigned int unit);
	void bindTexture() const;

	unsigned int getTexHandle() const;
	unsigned int getTexUnit() const;
};
