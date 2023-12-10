#pragma once

//#include "Mesh.h"
#include "../Shader.h"

class Mesh;
class Model;

class Node
{
private:
	std::vector<Mesh> meshes;
	glm::mat4 transformationMatrix;

	std::vector<Node> childNodes;

	Mesh ProcessMesh(aiMesh* assimpMesh, aiScene* scene);

	std::shared_ptr<Model> modelRef;

public:
	Node(std::shared_ptr<Model> model);

	void ProcessNodeHierarchy(aiNode* node, aiScene* scene);
	void RenderNodeHierarchy(Shader *shader);


};