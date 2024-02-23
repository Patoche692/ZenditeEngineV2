#pragma once
#include "../Camera.h"
#include "../utils.h"
#include "../ECS/ECSUtils.h"

class ECSCoordinator;
class c_Transform;
class c_AABB;

class I_Renderer
{
protected:
	std::shared_ptr<Camera> cam;


public:
	I_Renderer(std::shared_ptr<Camera> camera)
	{
		cam = camera;
	}

	virtual void Render(const R_DataHandle& DataHandle, const c_Transform& trans) = 0;
	virtual void RenderAABB(const R_DataHandle& DataHandle, 
		Shader& AABBShader,
		const c_AABB& AABB_Data,
		const c_Transform& trans) = 0;
	virtual void RenderLighting(const R_DataHandle& DataHandle,
		const c_Transform& trans,
		std::shared_ptr<ECSCoordinator> ECScoord) = 0;


};