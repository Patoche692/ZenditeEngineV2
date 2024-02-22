#pragma once

#include "../ECSCoordinator.h"
#include "../I_System.h"
#include "../../EventManager/I_Subject.h"
#include "../../API_Rendering/I_Renderer.h"
#include "../../API_Rendering/I_API_Manager.h"
#include "../../ECS/Components.h"

#include "../../Shader.h"

class RenderAABBSystem : public I_System, public I_Subject
{
private:
	Shader m_AABBShader;

public:
	//res/shaders/AABB/vs_BasicAABB.glsl //res/shaders/AABB/fs_BasicAABB.glsl
	RenderAABBSystem() : m_AABBShader("res/shaders/AABB/vs_BasicAABB.glsl", "res/shaders/AABB/fs_BasicAABB.glsl")
	{
		
	}

	void NotifyObservers(std::shared_ptr<ECSCoordinator> ECS_Coord, int* i) override
	{
		for (size_t i = 0; i < observerList.size(); ++i) //Iterate through every item in observerList
		{
			//#To_Complete_3
		}
	}

	void RenderAABBs(std::shared_ptr<I_Renderer> renderer, std::shared_ptr<I_API_Manager> apiManager, std::shared_ptr<ECSCoordinator> ECScoord)
	{
		for (auto const& EID : m_EntitySet)
		{
			//If entities "modified" component is true, then call
			if (ECScoord->GetComponentDataFromEntity<c_Modified>(EID).isModifed)
			{
				apiManager->SetupAABBRenderData(EID, ECScoord);
				//apiManager->SetShaderForDataHandle(EID);

			}

			renderer->RenderAABB(apiManager->GetEntityDataHandle(EID),
				m_AABBShader,
				ECScoord->GetComponentDataFromEntity<c_AABB>(EID),
				ECScoord->GetComponentDataFromEntity<c_Transform>(EID));
		}
	}

private:

};