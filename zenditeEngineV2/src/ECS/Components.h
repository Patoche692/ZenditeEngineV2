#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Helper Data Types:

#define MAX_BONE_INFLUENCE 4

struct Vertex {
	Vertex()
	{

	}

	Vertex(glm::vec3 pos,
		glm::vec3 norm,
		glm::vec2 txCoord,
		glm::vec3 tang,
		glm::vec3 btan)
		:
		Position(pos),
		Normal(norm),
		TexCoords(txCoord),
		Tangent(tang),
		Bitangent(btan)
	{ }

	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;

	//bone indexes which will influence this vertex
	int m_BoneIDs[MAX_BONE_INFLUENCE];
	//weights from each bone
	float m_Weights[MAX_BONE_INFLUENCE];
};

struct LightweightVertex
{
	glm::vec3 Position;
};


//COMPONENTS:

struct c_Transform
{
	//std::vector<glm::vec3> pos;
	//glm::vec3 prevPos;
	//glm::vec3 scale;

	glm::mat4 prevModelMat;
	std::vector<glm::mat4> modelMat;
};

struct c_AABB
{
	glm::vec3 scale;
	float *vertices; //Data to go in the VBO that stores the vertices of the AABB
	bool isColliding;
	bool isWallColliding;
};

struct c_Wall
{

};

struct c_WallCollider
{

};

struct c_Renderable
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
};

struct c_Texture
{
	unsigned short int texUnit;
	//std::string type;
};

struct c_LightRenderable
{
	std::vector<LightweightVertex> vertices;
	std::vector<unsigned int> indices;
	bool active;
};

struct c_DirLightEmitter
{
    glm::vec3 direction;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

	unsigned int depthMapFBO;
	unsigned int depthMapUnit;
};

struct c_PointLightEmitter
{
    float constant;
    float linear;
    float quadratic;
	
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

struct c_SpotLightEmitter
{
    glm::vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;       

};

struct c_EntityInfo
{
	std::string name;

	
};

struct c_Modified
{
	bool isModifed;
};