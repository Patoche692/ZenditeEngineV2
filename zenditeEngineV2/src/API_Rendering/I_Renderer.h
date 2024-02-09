#pragma once
#include "../Camera.h"
#include "../utils.h"
#include "../ECS/ECSUtils.h"

class ECSCoordinator;

class I_Renderer
{
protected:
	Camera cam;


public:
	virtual void Render(const R_DataHandle& DataHandle, const transform& trans) = 0;

};