#pragma once

//#include "Mesh.h"
#include "../Shader.h"

#include "Mesh.h"

class Model;

class Node
{
private:
	std::vector<Mesh> meshes;
	glm::mat4 transformationMatrix;

	std::vector<Node> childNodes;

	Mesh ProcessMesh(aiMesh* assimpMesh, const aiScene* scene);

	Model* modelRef;

public:
	Node(Model* model);

	void ProcessNodeHierarchy(aiNode* node, const aiScene* scene);
	void RenderNodeHierarchy(Shader *shader);


};