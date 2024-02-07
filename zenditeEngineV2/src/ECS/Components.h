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

};

struct c_RigidBody
{

};

struct c_ImmovableCollidable
{

};

struct c_Modified
{
	bool isModifed;
};