#pragma once

#include "../ECSCoordinator.h"
#include "../I_System.h"
#include "../../EventManager/I_Subject.h"
#include "../../API_Rendering/I_Renderer.h"
#include "../../API_Rendering/I_API_Manager.h"
#include "../../ECS/Components.h"

class RenderLightingSystem : public I_System, public I_Subject
{
private:
	//std::shared_ptr<ECSCoordinator> ECScoord;



public:
	RenderLightingSystem()
	{
		
	}

	void NotifyObservers(std::shared_ptr<ECSCoordinator> ECS_Coord, int* i) override
	{
		for (size_t i = 0; i < observerList.size(); ++i) //Iterate through every item in observerList
		{

		}
	}

	void Render(std::shared_ptr<I_Renderer> renderer, std::shared_ptr<I_API_Manager> apiManager, std::shared_ptr<ECSCoordinator> ECScoord)
	{	
		for (auto const& EID : m_EntitySet)
		{
			//If entities "modified" component is true, then call
			if (ECScoord->GetComponentDataFromEntity<c_Modified>(EID).isModifed)
			{
				apiManager->SetupLightingRenderData(EID, ECScoord);
				apiManager->SetShaderForDataHandle(EID);

			}

			renderer->RenderLighting(apiManager->GetEntityDataHandle(EID), ECScoord->GetComponentDataFromEntity<c_Transform>(EID), ECScoord);
		}
	}

private:

};
