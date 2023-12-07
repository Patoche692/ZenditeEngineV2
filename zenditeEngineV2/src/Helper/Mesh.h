#pragma once
#include "../utils.h"
#include <vector>

class Shader;
//class Texture2D;

struct Texture 
{
	unsigned int id; //texture handle
	std::string type;
	std::string path;
};

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

class Mesh
{
private:
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

	void setupMesh();

public:
	std::vector<Vertex> vertex;
	std::vector<unsigned int> index;
	std::vector<Texture> texture;

	//Constructor
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void Draw(Shader& shader);

};