#pragma once

#include "../ECSCoordinator.h"
#include "../I_System.h"
#include "../../EventManager/I_Subject.h"
#include "../../API_Rendering/I_Renderer.h"
#include "../../API_Rendering/I_API_Manager.h"
#include "../../ECS/Components.h"

class SetupPointLightSystem : public I_System, public I_Subject
{
private:
	//std::shared_ptr<ECSCoordinator> ECScoord;



public:
	SetupPointLightSystem()
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
		std::vector<Entity> pointLights;
		for (auto const& EID : m_EntitySet)
		{
			pointLights.push_back(EID);
		}
		ECScoord->SetSpotLightEntities(pointLights);
	}

private:

};
