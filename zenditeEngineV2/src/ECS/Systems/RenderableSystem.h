#pragma once

#include "../ECSCoordinator.h"
#include "../I_System.h"
#include "../../EventManager/I_Subject.h"
#include "../../API_Rendering/I_Renderer.h"
#include "../../API_Rendering/I_API_Manager.h"

class RenderableSystem : public I_System, public I_Subject
{
private:
	std::shared_ptr<ECSCoordinator> ECScoord;

public:
	RenderableSystem(std::shared_ptr<ECSCoordinator> ECScoordPtr) : ECScoord(ECScoordPtr)
	{
		
	}

	void NotifyObservers(int* i) override
	{
		for (size_t i = 0; i < observerList.size(); ++i) //Iterate through every item in observerList
		{

		}
	}

	void Render(std::shared_ptr<I_Renderer> renderer, std::shared_ptr<I_API_Manager> apiManager)
	{	
		for (auto const& EID : m_EntitySet)
		{
			//If entities "modified" component is true, then call
			if (true)
			{
				apiManager->SetupRenderData(EID);
			}

			renderer->Render(apiManager->GetEntityDataHandle(EID), ECScoord->GetComponentDataFromEntity<transform>(EID));
		}
	}

private:

};