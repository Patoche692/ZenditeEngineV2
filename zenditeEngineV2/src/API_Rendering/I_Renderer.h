#pragma once
#include "../Camera.h"
#include "../utils.h"
#include "../ECS/ECSUtils.h"

class ECSCoordinator;
class c_Transform;

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

};