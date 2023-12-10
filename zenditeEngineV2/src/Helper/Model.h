#pragma once
//#include "Node.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <memory>

#include "../Shader.h"

#include "Node.h"
#include "Texture.h"

class Model
{
private:
	std::string filePath;
	Assimp::Importer importer;
	aiScene* assimpSceneObj;
	std::vector<Texture> loaded_textures;

	//Smart ptr version of: Node* rootNode;
	std::unique_ptr<Node> rootNode;
	Shader* shaderHandle;

public:
	Model(const char* modelFilePath, Shader shader);
	void Draw(Shader shaderHandle);

	const std::vector<Texture>& getLoadedTexRef() const;
	std::string getObjFilePath() const;
	void addLoadedTexture(Texture newTexture);

	//Each model uses a single shader for now.
	


	//~Model(); //Delete rootNode if not using a smart pointer. Otherwise, delte importer maybe;
};