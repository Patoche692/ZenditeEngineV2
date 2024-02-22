#pragma once

#include "../ECSCoordinator.h"
#include "../I_System.h"
#include "../../EventManager/I_Subject.h"

class SetUpWallAABBSystem : public I_System, public I_Subject
{
private:
	unsigned int m_NoEntitiesInSystem;

	//std::shared_ptr<ECSCoordinator> ECScoord;

	void NotifyObservers(std::shared_ptr<ECSCoordinator> ECS_Coord, int* i) override
	{
		for (size_t j = 0; j < observerList.size(); ++j) //Iterate through every item in observerList
		{
			observerList[j]->UpdateObservers(ECS_Coord, i);
		}

		//delete[] i;
	}

public:
	//res/shaders/AABB/vs_BasicAABB.glsl //res/shaders/AABB/fs_BasicAABB.glsl
	SetUpWallAABBSystem()
	{
		m_NoEntitiesInSystem = 0;
		
	}

	void InitialSetup(std::shared_ptr<ECSCoordinator> ECScoord)
	{
		ECScoord->SetWallAABBEntitiesSetPtr(&m_EntitySet);
	}

	void Setup(std::shared_ptr<ECSCoordinator> ECScoord)
	{
		if (m_NoEntitiesInSystem != m_EntitySet.size())
		{
			ECScoord->SetWallAABBEntitiesSetPtr(&m_EntitySet);

			m_NoEntitiesInSystem = m_EntitySet.size();
		}
	}

};