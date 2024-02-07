#pragma once
#include "../utils.h"
#include "../ECS/ECSUtils.h"

class I_Renderer
{
protected:

public:
	virtual void Render(const R_DataHandle& DataHandle, const transform& trans) = 0;

};