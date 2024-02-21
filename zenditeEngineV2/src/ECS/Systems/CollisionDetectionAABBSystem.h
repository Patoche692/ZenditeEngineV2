#pragma once

#include "../ECSCoordinator.h"
#include "../I_System.h"
#include "../../EventManager/I_Subject.h"
#include "../../API_Rendering/I_Renderer.h"
#include "../../API_Rendering/I_API_Manager.h"
#include "../../ECS/Components.h"

#include "../../Shader.h"


class CollisionDetectionAABBSystem : public I_System, public I_Subject
{
private:
	std::shared_ptr<ECSCoordinator> ECScoord;

	void NotifyObservers(std::shared_ptr<ECSCoordinator> ECS_Coord, int* i) override
	{
		for (size_t j = 0; j < observerList.size(); ++j) //Iterate through every item in observerList
		{
			observerList[j].UpdateObservers(ECS_Coord, i);
		}

		//delete[] i;
	}

	//Will return true if a collision is occurring, false if not
	bool testCollision(Entity E0, Entity E1)
	{
		//#CONTINUE_HERE
	}

public:
	//res/shaders/AABB/vs_BasicAABB.glsl //res/shaders/AABB/fs_BasicAABB.glsl
	CollisionDetectionAABBSystem()
	{

	}

	void checkCollisions(std::shared_ptr<I_API_Manager> apiManager, std::shared_ptr<ECSCoordinator> ECScoord)
	{
		//Check all entites that are colliding;

		int NoOfEntitesColliding = 0;

		//#Parallelism_Potential
		for (std::set<std::uint32_t>::iterator it = m_EntitySet.begin(); it != m_EntitySet.end(); ++it)
		{
			for (std::set<std::uint32_t>::iterator iit = it; iit != m_EntitySet.end(); ++iit)
			{
				if(*iit == *it)
				{
					//do nothing
				}
				else
				{
					bool isColliding = testCollision(*it, *iit);

					if(isColliding)
					{
						int collidingEnts[2];
						collidingEnts[0] = *it;
						collidingEnts[1] = *iit;

						(ECScoord->GetComponentDataFromEntity<c_AABB>(*it)).isColliding = true;
						(ECScoord->GetComponentDataFromEntity<c_AABB>(*iit)).isColliding = true;

						NotifyObservers(ECScoord, collidingEnts);
					}
					else
					{
						(ECScoord->GetComponentDataFromEntity<c_AABB>(*it)).isColliding = false;
						(ECScoord->GetComponentDataFromEntity<c_AABB>(*iit)).isColliding = false;
					}
				}
			}
		}
	}

};