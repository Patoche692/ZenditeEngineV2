#pragma once
#include "ECS/ECSCoordinator.h"

class Coordinator
{
private:
	std::shared_ptr<ECSCoordinator> m_ECSCoord;

public:
	std::shared_ptr<ComponentManager> GetComponentManager()
	{
		return m_ECSCoord->GetComponentManager();
	}

};