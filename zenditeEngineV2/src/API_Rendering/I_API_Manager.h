#pragma once
#include "../utils.h"
#include "../ECS/ECSUtils.h"
#include "../ECS/ECSCoordinator.h"
#include <unordered_map>

class I_API_Manager
{
protected:
	std::unordered_map<Entity, R_DataHandle> m_Map_ENTITYtoHANDLE;

	virtual R_DataHandle& GetNonConstEntityDataHandle(Entity EID) = 0; 

public:
	virtual void SetupRenderData(Entity EID, std::shared_ptr<ECSCoordinator> ECScoord) = 0;
	virtual R_DataHandle const& GetEntityDataHandle(Entity EID) const = 0;
	virtual void SetShaderForEntity(Entity EID, std::shared_ptr<Shader> shader) = 0;


};