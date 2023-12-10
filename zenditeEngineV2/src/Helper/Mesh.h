#pragma once
#include "../utils.h"

#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../vendor/stb_image/stb_image.h"

#include "../Shader.h"
#include "Texture.h"

//#include "Model.h"
class Model;

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
	Model* modelRef;

	aiMaterial* assimpMaterial;
	aiMesh* assimpMesh;
	const aiScene* assimpScene;

	std::vector<Texture> diffuseTextures;
	std::vector<Texture> specularTextures;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
	//Tex handle is stored in the texture structure.

	void processVertPositionData(Vertex* vert, unsigned int i);
	void processVertNormalData(Vertex* vert, unsigned int i);
	void processVertTexCoords(Vertex* vert, unsigned int i);
	void processVertices();

	void processIndices();

	void processTextures();

	std::vector<Texture> loadSpecularTextureFromMaterial();

	std::vector<Texture> loadDiffuseTextureFromMaterial();

	unsigned int createGLTextureBuffer(const char* filePath);

	void processDataForOpenGl();

public:
	Mesh(aiMesh* mesh, const aiScene* scene, Model* model); //scene is needed when you want to retrieve the textures from the material.

	void DrawMesh(Shader* shader);
};