#pragma once
#include "I_Renderer.h"

class ECSCoordinator;

class OpenGL_Renderer : public I_Renderer
{
public:
	OpenGL_Renderer(std::shared_ptr<Camera> cam);
	void Render(const R_DataHandle& DataHandle, ECSCoordinator& ECScoord, Entity EID) override;
	void RenderAABB(const R_DataHandle& DataHandle, 
		Shader& AABBShader,
		const c_AABB& AABB_Data, 
		const c_Transform& trans) override;
	void RenderShadowMap(const R_DataHandle& DataHandle, ECSCoordinator& ECScoord, Shader& shader, Entity EID) override;
	void RenderLightSource(const R_DataHandle& DataHandle, ECSCoordinator& ECScoord, Entity EID, Shader& shader, glm::vec3 lightColor) override;

private:
	
};