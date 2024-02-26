#include "../utils.h"
#include "OpenGl_Renderer.h"
#include "../ECS/ECSCoordinator.h"
#include "../Shader.h"
#include "../geometrySetup.h"
#include "../TextureData.h"
#include "../ECS/Components.h"

OpenGL_Renderer::OpenGL_Renderer(std::shared_ptr<Camera> cam) : I_Renderer(cam)
{
	
}

void OpenGL_Renderer::Render(const R_DataHandle& DataHandle, const c_Transform& trans)
{
	(DataHandle.shader)->bindProgram();
	bindVao(DataHandle.VAO);
	glm::mat4 cubeProjection = glm::perspective(glm::radians(cam->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 cubeView = cam->GetViewMatrix();
	(DataHandle.shader)->setUniformMat4("projection", GL_FALSE, glm::value_ptr(cubeProjection));
	(DataHandle.shader)->setUniformMat4("view", GL_FALSE, glm::value_ptr(cubeView));

	glm::mat4 cubeModel = glm::mat4(1.0f);
	cubeModel = glm::translate(cubeModel, trans.pos);
	cubeModel = glm::scale(cubeModel, trans.scale);
	(DataHandle.shader)->setUniformMat4("model", GL_FALSE, glm::value_ptr(cubeModel));

	//(DataHandle.texture)->changeTexUnit(DataHandle.texUnit); //#unnecessary. Each texture is saved to a texture unit and is not changed throught the programs lifespan
															   //			   This might be useful later if assigned texture units can be modified later during runtime
															   //			   Although, all this does is take a texture and assign it to a texture unit.

	

	(DataHandle.shader)->setUniformTextureUnit("colorTexture", DataHandle.texUnit);

	GLCALL(glDrawArrays(GL_TRIANGLES, 0, 36));
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

	glm::mat4 cubeModel = glm::mat4(1.0f);
	cubeModel = glm::translate(cubeModel, trans.pos);
	cubeModel = glm::scale(cubeModel, AABB_Data.scale);
	AABBShader.setUniformMat4("model", GL_FALSE, glm::value_ptr(cubeModel));

	AABBShader.setUniform4f("lineColor", 1.0f, 0.08f, 0.58f, 1.0f);

	GLCALL(glDrawArrays(GL_LINES, 0, 24));

}
