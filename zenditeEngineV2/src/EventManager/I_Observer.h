#pragma once

#include "../ECS/ECSCoordinator.h"

//#TODO_NOTE: Use GetEntitySignature from ECS coordinator when you pass in an array of entites to filter out unwanted ones.

class I_Observer
{
public:
	virtual void UpdateObservers(std::shared_ptr<ECSCoordinator> ECS_Coord, int* i) = 0;
};