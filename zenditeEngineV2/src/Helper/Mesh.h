#pragma once
#include "../utils.h"

#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Model.h"


struct Vertex
{
	//Position
	glm::vec3 pos;

	//Normal
	glm::vec3 norm;

	//TexCoords
	glm::vec2 texCord;

	//And additional parameters you might want.
};

class Mesh
{
private:
	std::shared_ptr<Model> modelRef;

	aiMaterial* assimpMaterial;
	aiMesh* assimpMesh;
	aiScene* assimpScene;

	std::vector<Texture> diffuseTextures;
	std::vector<Texture> specularTextures;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

	void processVertPositionData(Vertex* vert, unsigned int i);
	void processVertNormalData(Vertex* vert, unsigned int i);
	void processVertTexCoords(Vertex* vert, unsigned int i);
	void processVertices();

	void processIndices();

	void processTextures();

	std::vector<Texture> loadSpecularTextureFromMaterial();
	std::vector<Texture> loadDiffuseTextureFromMaterial();

	unsigned int createGLTextureBuffer(const char* filePath);

public:
	Mesh(aiMesh* mesh, aiScene* scene, std::shared_ptr<Model> model) : modelRef(model) //scene is needed when you want to retrieve the textures from the material.
	{
		//Store input data:
		assimpMesh = mesh;
		assimpScene = scene;
		
		//Collect vertex data:
		processVertices();

		//Collect Indices:
		processIndices();

		//Collect Textures from materials;
		processTextures();

		//Assign data to VAO, VBO and EBO and store data in VRAM for later usage;
	}

	void processMesh();
};