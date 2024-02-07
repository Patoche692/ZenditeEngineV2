#pragma once
#include "../I_System.h"
#include "../../EventManager/I_Subject.h"

class CollisionDetectionSystem : public I_System, public I_Subject
{
public:
	void NotifyObservers(int* i) override
	{
		for (size_t i = 0; i < observerList.size(); ++i) //Iterate through every item in observerList
		{
			
		}
	}

private:

};