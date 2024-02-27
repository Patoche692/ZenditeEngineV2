#pragma once

#include "../ECSCoordinator.h"
#include "../I_System.h"
#include "../../EventManager/I_Subject.h"
#include "../../API_Rendering/I_Renderer.h"
#include "../../API_Rendering/I_API_Manager.h"
#include "../../ECS/Components.h"

class SetupDirLightSystem : public I_System, public I_Subject
{
private:
	//std::shared_ptr<ECSCoordinator> ECScoord;



public:
	SetupDirLightSystem()
	{
		//#Temp_Simple_Rendering_System
		
	}

	void NotifyObservers(std::shared_ptr<ECSCoordinator> ECS_Coord, int* i) override
	{
		for (size_t i = 0; i < observerList.size(); ++i) //Iterate through every item in observerList
		{

		}
	}

	void Setup(std::shared_ptr<I_API_Manager> apiManager, std::shared_ptr<ECSCoordinator> ECScoord)
	{	
		
		for (auto const& EID : m_EntitySet)
		{
			
			//If entities "modified" component is true, then call

			//Set up each point light here ------------------------:

		}

		ECScoord->SetDirLightEntitiesPtr(&m_EntitySet);
	}

private:

};