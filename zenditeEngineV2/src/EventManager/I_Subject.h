#pragma once
#include <vector>
#include "I_Observer.h"

class I_Subject
{
protected:
	std::vector<I_Observer> observerList;

public:
	virtual void NotifyObservers(std::shared_ptr<ECSCoordinator> ECS_Coord, int* i) = 0;
};