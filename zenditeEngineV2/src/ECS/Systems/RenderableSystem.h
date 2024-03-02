#pragma once

#include "../ECSCoordinator.h"
#include "../I_System.h"
#include "../../EventManager/I_Subject.h"
#include "../../API_Rendering/I_Renderer.h"
#include "../../API_Rendering/I_API_Manager.h"
#include "../../ECS/Components.h"

class RenderableSystem : public I_System, public I_Subject
{
private:
	//std::shared_ptr<ECSCoordinator> ECScoord;
	Shader depthShader;


public:
	RenderableSystem() : depthShader("res/shaders/Shadows/vs_shadowMap.glsl", "res/shaders/Shadows/fs_shadowMap.glsl")
	{
		//#Temp_Simple_Rendering_System
		
	}

	void NotifyObservers(std::shared_ptr<ECSCoordinator> ECS_Coord, int* i) override
	{
		for (size_t i = 0; i < observerList.size(); ++i) //Iterate through every item in observerList
		{
			//#To_Complete_4
		}
	}

	void Render(std::shared_ptr<I_Renderer> renderer, std::shared_ptr<I_API_Manager> apiManager, std::shared_ptr<ECSCoordinator> ECScoord)
	{	
		for (auto const& EID : m_EntitySet)
		{
			//If entities "modified" component is true, then call
			if (ECScoord->GetComponentDataFromEntity<c_Modified>(EID).isModifed)
			{
				apiManager->SetupRenderData(EID, ECScoord);
				apiManager->SetShaderForDataHandle(EID);

			}
		}
		RenderShadowMaps(renderer, apiManager, ECScoord);
		for (auto const& EID : m_EntitySet)
		{
			renderer->Render(apiManager->GetEntityDataHandle(EID), *ECScoord, EID);
		}
	}

	void RenderShadowMaps(std::shared_ptr<I_Renderer> renderer, std::shared_ptr<I_API_Manager> apiManager, std::shared_ptr<ECSCoordinator> ECScoord)
	{
		glViewport(0, 0, SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT);

		depthShader.bindProgram();

		std::set<Entity>* DirLightSet = ECScoord->GetDirLightEntitiesPtr();
		glm::mat4 lightProjection, lightView, lightSpaceMatrix;
		for (std::set<std::uint32_t>::iterator it = (*DirLightSet).begin(); it != (*DirLightSet).end(); ++it)
		{
			c_Transform& dirLightMM = ECScoord->GetComponentDataFromEntity<c_Transform>(*it);

			glm::vec3 dirLightTransform;
			dirLightTransform.x = dirLightMM.modelMat[0][3][0];
			dirLightTransform.y = dirLightMM.modelMat[0][3][1];
			dirLightTransform.z = dirLightMM.modelMat[0][3][2];

			c_DirLightEmitter& dirLightData = ECScoord->GetComponentDataFromEntity<c_DirLightEmitter>(*it);
			lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 20.0f);
			lightView = glm::lookAt(dirLightTransform, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			lightSpaceMatrix = lightProjection * lightView;

			depthShader.setUniformMat4("lightSpaceMatrix", GL_FALSE, glm::value_ptr(lightSpaceMatrix));

			glBindFramebuffer(GL_FRAMEBUFFER, dirLightData.depthMapFBO);
			glClear(GL_DEPTH_BUFFER_BIT);
			for (auto const& EID : m_EntitySet)
			{
				renderer->RenderShadowMap(apiManager->GetEntityDataHandle(EID), *ECScoord, depthShader, EID);
			}
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		std::set<Entity>* SpotLightSet = ECScoord->GetSpotLightEntitiesPtr();
		for (std::set<std::uint32_t>::iterator it = (*SpotLightSet).begin(); it != (*SpotLightSet).end(); ++it)
		{
			c_Transform& spotLightMM = ECScoord->GetComponentDataFromEntity<c_Transform>(*it);

			glm::vec3 spotLightTransform;
			spotLightTransform.x = spotLightMM.modelMat[0][3][0];
			spotLightTransform.y = spotLightMM.modelMat[0][3][1];
			spotLightTransform.z = spotLightMM.modelMat[0][3][2];

			c_SpotLightEmitter& spotLightData = ECScoord->GetComponentDataFromEntity<c_SpotLightEmitter>(*it);
			lightProjection = glm::perspective((spotLightData.outerCutOff), 1.0f, 0.01f, 100.0f);
			lightView = glm::lookAt(spotLightTransform, spotLightTransform + spotLightData.direction, glm::vec3(0.0f, 1.0f, 0.0f));
			lightSpaceMatrix = lightProjection * lightView;

			depthShader.setUniformMat4("lightSpaceMatrix", GL_FALSE, glm::value_ptr(lightSpaceMatrix));

			glBindFramebuffer(GL_FRAMEBUFFER, spotLightData.depthMapFBO);
			glClear(GL_DEPTH_BUFFER_BIT);
			for (auto const& EID : m_EntitySet)
			{
				renderer->RenderShadowMap(apiManager->GetEntityDataHandle(EID), *ECScoord, depthShader, EID);
			}
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	}

private:

};