#pragma once

#include "../ECS/ECSCoordinator.h"

class I_Observer
{
public:
	virtual void UpdateObservers(std::shared_ptr<ComponentManager> CM, int* i);
};