#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct c_Transform
{
	glm::vec3 pos;
	glm::vec3 scale;
};

struct c_RenderableComponent
{
	void setVertexArray(float* verts, size_t size)
	{
		vertices = new float[size];

		for (int i = 0; i < size; ++i)
		{
			vertices[i] = verts[i];
		}

		arraySize = size;
	}

	float* vertices;
	size_t arraySize;
};

struct c_RigidBodyCollidable
{

};

struct c_ImmovableCollidable
{

};

struct c_CollidableTrigger
{

};

struct c_Modified
{
	bool isModifed;
};