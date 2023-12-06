#pragma once
#include "../utils.h"
#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
private:
	std::vector<Mesh> mesh;
	std::string directory;
	std::vector<Texture2D> textures_loaded;

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture2D> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);


public:
	Model(const char* path)
	{
		loadModel(path);
	}

	void Draw(Shader& shader);
};