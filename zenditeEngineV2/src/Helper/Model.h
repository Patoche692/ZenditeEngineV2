#pragma once
//#include "Node.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <memory>

class Shader;
class Node;

struct Texture
{
	unsigned int texHandle;
	std::string type; //diffuse, specular, etc.
	std::string fullPath;
};

class Model
{
private:
	std::string filePath;
	Assimp::Importer importer;
	aiScene* assimpSceneObj;
	std::vector<Texture> loaded_textures;

	//Smart ptr version of: Node* rootNode;
	std::unique_ptr<Node> rootNode;

public:
	Model(const char* modelFilePath)
	{
		filePath = modelFilePath;
	}
	void Draw(Shader shaderHandle);

	const std::vector<Texture>& getLoadedTexRef() const;
	std::string getObjFilePath() const;
	void addLoadedTexture(Texture newTexture);

	~Model(); //Delete rootNode if not using a smart pointer. Otherwise, delte importer maybe;
};