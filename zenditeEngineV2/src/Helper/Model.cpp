#include "Model.h"
#include "Node.h"
#include "../Shader.h"


void Model::Draw(Shader shaderHandle)
{

}

const std::vector<Texture>& Model::getLoadedTexRef() const
{
	return loaded_textures;
}

std::string Model::getObjFilePath() const
{
	return filePath;
}

void Model::addLoadedTexture(Texture newTexture)
{
	loaded_textures.push_back(newTexture);
}

Model::~Model()
{

}
