#include "Node.h"

Mesh Node::ProcessMesh(aiMesh* assimpMesh, aiScene* scene)
{
	Mesh meshObj(assimpMesh, scene, modelRef); //Mesh can handle its own creation
	
	//Add any additional set up functions here!
	
	return meshObj;
}

void Node::ProcessNodeHierarchy(aiNode* node, aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++) // for each mesh in the node
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]]; // retrieve the index reference and store the data from scene in assimp mesh.

		meshes.push_back(ProcessMesh(mesh, scene));

	}
	//Then do the same for every child node (if there are any)
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		Node childNode(modelRef);
		childNode.ProcessNodeHierarchy(node->mChildren[i], scene);
		childNodes.push_back(childNode);
	}
}


void Node::RenderNodeHierarchy()
{

}



