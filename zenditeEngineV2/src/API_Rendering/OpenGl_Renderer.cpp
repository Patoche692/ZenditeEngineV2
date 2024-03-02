#include "../utils.h"
#include "OpenGl_Renderer.h"
#include "../ECS/ECSCoordinator.h"
#include "../geometrySetup.h"
#include "../TextureData.h"
#include "../ECS/Components.h"

OpenGL_Renderer::OpenGL_Renderer(std::shared_ptr<Camera> cam) : I_Renderer(cam)
{
	
}

void OpenGL_Renderer::Render(const R_DataHandle& DataHandle, ECSCoordinator& ECScoord, Entity EID)
{
	c_Transform& trans = ECScoord.GetComponentDataFromEntity<c_Transform>(EID);
	c_Renderable& rendData = ECScoord.GetComponentDataFromEntity<c_Renderable>(EID);

	std::shared_ptr<Shader> shader = DataHandle.shader;

	shader->bindProgram();
	bindVao(DataHandle.VAO);
	glm::mat4 cubeProjection = glm::perspective(glm::radians(cam->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 cubeView = cam->GetViewMatrix();
	shader->setUniformMat4("projection", GL_FALSE, glm::value_ptr(cubeProjection));
	shader->setUniformMat4("view", GL_FALSE, glm::value_ptr(cubeView));

	shader->setUniformMat4("model", GL_FALSE, glm::value_ptr(trans.modelMat[0]));

	std::set<Entity>* DirLightSet = ECScoord.GetDirLightEntitiesPtr();
	int nrDirLights = DirLightSet->size();
	shader->setUniformInt("nrDirLights", nrDirLights);

	int i = 0;
	glm::mat4 lightProjection, lightView, lightSpaceMatrix;
	for (std::set<std::uint32_t>::iterator it = (*DirLightSet).begin(); it != (*DirLightSet).end(); ++it, i++)
	{
		c_DirLightEmitter& dirLightData = ECScoord.GetComponentDataFromEntity<c_DirLightEmitter>(*it);
		c_Transform& dirLightMM = ECScoord.GetComponentDataFromEntity<c_Transform>(*it);
		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 20.0f);

		glm::vec3 dirLightTransform;
		dirLightTransform.y = dirLightMM.modelMat[0][3][1];
		dirLightTransform.x = dirLightMM.modelMat[0][3][0];
		dirLightTransform.z = dirLightMM.modelMat[0][3][2];

		lightView = glm::lookAt(dirLightTransform, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		lightSpaceMatrix = lightProjection * lightView;

		shader->setUniformMat4(("dirLightSpaceMatrixes[" + std::to_string(i) + "]"), GL_FALSE, glm::value_ptr(lightSpaceMatrix));
		shader->setUniform3fv(("dirLights[" + std::to_string(i) + "].position"), dirLightTransform);
		shader->setUniform3fv(("dirLights[" + std::to_string(i) + "].direction"), dirLightData.direction);
		shader->setUniform3fv(("dirLights[" + std::to_string(i) + "].ambient"), dirLightData.ambient);
		shader->setUniform3fv(("dirLights[" + std::to_string(i) + "].diffuse"), dirLightData.diffuse);
		shader->setUniform3fv(("dirLights[" + std::to_string(i) + "].specular"), dirLightData.specular);
		shader->setUniformTextureUnit(("dirLights[" + std::to_string(i) + "].shadowMap"), 8 + i);
		glActiveTexture(GL_TEXTURE8 + i);
		glBindTexture(GL_TEXTURE_2D, dirLightData.depthMapUnit);
	}
	
	std::set<Entity>* SpotLightSet = ECScoord.GetSpotLightEntitiesPtr();
	int nrSpotLights = SpotLightSet->size();
	shader->setUniformInt("nrSpotLights", nrSpotLights);

	i = 0;
	for (std::set<std::uint32_t>::iterator it = (*SpotLightSet).begin(); it != (*SpotLightSet).end(); ++it, i++)
	{
		c_SpotLightEmitter& spotLightData = ECScoord.GetComponentDataFromEntity<c_SpotLightEmitter>(*it);
		c_Transform& spotLightMM = ECScoord.GetComponentDataFromEntity<c_Transform>(*it);

		glm::vec3 spotLightTransform;
		spotLightTransform.x = spotLightMM.modelMat[0][3][0];
		spotLightTransform.y = spotLightMM.modelMat[0][3][1];
		spotLightTransform.z = spotLightMM.modelMat[0][3][2];

		shader->setUniform3fv(("spotLights[" + std::to_string(i) + "].position"), spotLightTransform);
		shader->setUniform3fv(("spotLights[" + std::to_string(i) + "].direction"), spotLightData.direction);
		shader->setUniformFloat(("spotLights[" + std::to_string(i) + "].cutOff"), spotLightData.cutOff);
		shader->setUniformFloat(("spotLights[" + std::to_string(i) + "].outerCutOff"), spotLightData.outerCutOff);
		shader->setUniformFloat(("spotLights[" + std::to_string(i) + "].constant"), spotLightData.constant);
		shader->setUniformFloat(("spotLights[" + std::to_string(i) + "].linear"), spotLightData.linear);
		shader->setUniformFloat(("spotLights[" + std::to_string(i) + "].quadratic"), spotLightData.quadratic);
		shader->setUniform3fv(("spotLights[" + std::to_string(i) + "].ambient"), spotLightData.ambient);
		shader->setUniform3fv(("spotLights[" + std::to_string(i) + "].diffuse"), spotLightData.diffuse);
		shader->setUniform3fv(("spotLights[" + std::to_string(i) + "].specular"), spotLightData.specular);
	}

	std::set<Entity>* PointLightSet = ECScoord.GetPointLightEntitiesPtr();
	int nrPointLights = PointLightSet->size();
	shader->setUniformInt("nrPointLights", nrPointLights);

	i = 0;
	for (std::set<std::uint32_t>::iterator it = (*PointLightSet).begin(); it != (*PointLightSet).end(); ++it, ++i)
	{
		c_PointLightEmitter& pointLightData = ECScoord.GetComponentDataFromEntity<c_PointLightEmitter>(*it);
		c_Transform& pointLightMM = ECScoord.GetComponentDataFromEntity<c_Transform>(*it);

		glm::vec3 pointLightTransform;
		pointLightTransform.x = pointLightMM.modelMat[0][3][0];
		pointLightTransform.y = pointLightMM.modelMat[0][3][1];
		pointLightTransform.z = pointLightMM.modelMat[0][3][2];

		shader->setUniform3fv(("pointLights[" + std::to_string(i) + "].position"), pointLightTransform);
		shader->setUniformFloat(("pointLights[" + std::to_string(i) + "].constant"), pointLightData.constant);
		shader->setUniformFloat(("pointLights[" + std::to_string(i) + "].linear"), pointLightData.linear);
		shader->setUniformFloat(("pointLights[" + std::to_string(i) + "].quadratic"), pointLightData.quadratic);
		shader->setUniform3fv(("pointLights[" + std::to_string(i) + "].ambient"), pointLightData.ambient);
		shader->setUniform3fv(("pointLights[" + std::to_string(i) + "].diffuse"), pointLightData.diffuse);
		shader->setUniform3fv(("pointLights[" + std::to_string(i) + "].specular"), pointLightData.specular);
	}

	//(DataHandle.texture)->changeTexUnit(DataHandle.texUnit); //#unnecessary. Each texture is saved to a texture unit and is not changed throught the programs lifespan
															   //			   This might be useful later if assigned texture units can be modified later during runtime
															   //			   Although, all this does is take a texture and assign it to a texture unit.
	glm::vec3 camPosition = cam->getPosition();
	shader->setUniform3fv("viewPos", camPosition);


	shader->setUniformTextureUnit("colorTexture", 0);
	shader->setUniformTextureUnit("material.diffuse", 0);
	shader->setUniform3fv("material.specular", 0.5f, 0.5f, 0.5f);
	shader->setUniformFloat("material.shininess", 32.0f);
	glActiveTexture(GL_TEXTURE0);
	// To fix: texture units starting from 1
	glBindTexture(GL_TEXTURE_2D, DataHandle.texUnit + 1);

	for (int i = 0; i < trans.modelMat.size(); ++i)
	{
		(DataHandle.shader)->setUniformMat4("model", GL_FALSE, glm::value_ptr((trans.modelMat)[i]));

		GLCALL(glDrawElements(GL_TRIANGLES, (rendData.indices).size(), GL_UNSIGNED_INT, 0));
	}
}

void OpenGL_Renderer::RenderAABB(const R_DataHandle& DataHandle, 
	Shader& AABBShader, 
	const c_AABB& AABB_Data, 
	const c_Transform& trans)
{
	AABBShader.bindProgram();
	bindVao(DataHandle.AABB_VAO);

	glm::mat4 cubeProjection = glm::perspective(glm::radians(cam->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 cubeView = cam->GetViewMatrix();
	AABBShader.setUniformMat4("projection", GL_FALSE, glm::value_ptr(cubeProjection));
	AABBShader.setUniformMat4("view", GL_FALSE, glm::value_ptr(cubeView));

	glm::vec3 AABBpos;

	// Always use position 0 in the first []
	AABBpos.x = trans.modelMat[0][3][0];
	AABBpos.y = trans.modelMat[0][3][1];
	AABBpos.z = trans.modelMat[0][3][2]; //#NOTE: We extract the x, y and z data to avoid also attempting to copy the rotation.

	glm::mat4 cubeModel = glm::mat4(1.0f);
	cubeModel = glm::translate(cubeModel, AABBpos);
	cubeModel = glm::scale(cubeModel, AABB_Data.scale);
	AABBShader.setUniformMat4("model", GL_FALSE, glm::value_ptr(cubeModel));

	if (AABB_Data.isColliding == true)
	{
		if (AABB_Data.isWallColliding == true)
		{
			AABBShader.setUniform4f("lineColor", 0.0f, 1.0f, 0.0f, 1.0f);
		}
		else
		{
			AABBShader.setUniform4f("lineColor", 1.0f, 0.0f, 0.0f, 1.0f);
		}
	}
	else
	{
		AABBShader.setUniform4f("lineColor", 1.0f, 0.08f, 0.58f, 1.0f);
	}

	GLCALL(glDrawArrays(GL_LINES, 0, 24));

}

void OpenGL_Renderer::RenderLightSource(const R_DataHandle& DataHandle, ECSCoordinator& ECScoord, Entity EID, Shader& shader, glm::vec3 lightColor)
{
	c_Transform& trans = ECScoord.GetComponentDataFromEntity<c_Transform>(EID);
	c_LightRenderable& rendData = ECScoord.GetComponentDataFromEntity<c_LightRenderable>(EID);

	//std::shared_ptr<Shader> lightSourceShader = DataHandle.shader;

	shader.bindProgram();
	bindVao(DataHandle.Light_VAO);
	glm::mat4 cubeProjection = glm::perspective(glm::radians(cam->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 cubeView = cam->GetViewMatrix();
	shader.setUniformMat4("projection", GL_FALSE, glm::value_ptr(cubeProjection));
	shader.setUniformMat4("view", GL_FALSE, glm::value_ptr(cubeView));
	shader.setUniformMat4("model", GL_FALSE, glm::value_ptr(trans.modelMat[0]));
 
	shader.setUniform3fv("lightRGB", lightColor.x, lightColor.y, lightColor.z);

	for (int i = 0; i < trans.modelMat.size(); ++i)
	{
		shader.setUniformMat4("model", GL_FALSE, glm::value_ptr((trans.modelMat)[i]));

		GLCALL(glDrawElements(GL_TRIANGLES, (rendData.indices).size(), GL_UNSIGNED_INT, 0));
	}
}

void OpenGL_Renderer::RenderShadowMap(const R_DataHandle& DataHandle, ECSCoordinator& ECScoord, Shader& shader, Entity EID)
{
	shader.bindProgram();

	bindVao(DataHandle.VAO);

	c_Transform& trans = ECScoord.GetComponentDataFromEntity<c_Transform>(EID);
	c_Renderable& rendData = ECScoord.GetComponentDataFromEntity<c_Renderable>(EID);

	for (int i = 0; i < trans.modelMat.size(); ++i)
	{
		shader.setUniformMat4("model", GL_FALSE, glm::value_ptr((trans.modelMat)[i]));

		GLCALL(glDrawElements(GL_TRIANGLES, (rendData.indices).size(), GL_UNSIGNED_INT, 0));
	}
}
