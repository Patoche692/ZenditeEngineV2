#pragma once
#include <vector>
#include "I_Observer.h"

class I_Subject
{
protected:
	std::vector<I_Observer> observerList;

public:
	virtual void NotifyObservers(int* i) = 0;
};