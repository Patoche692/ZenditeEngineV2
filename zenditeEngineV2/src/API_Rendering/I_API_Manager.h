#pragma once
#include "../ECS/ECSUtils.h"
#include <unordered_map>

class I_API_Manager
{
protected:
	std::unordered_map<Entity, R_DataHandle> m_Map_ENTITYtoHANDLE;

public:
	virtual void SetupRenderData(Entity EID) = 0;

};