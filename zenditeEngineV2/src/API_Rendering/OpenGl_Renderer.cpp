#include "../utils.h"
#include "OpenGl_Renderer.h"
#include "../ECS/ECSCoordinator.h"
#include "../Shader.h"
#include "../geometrySetup.h"
#include "../Texture2D.h"
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

	(DataHandle.texture)->changeTexUnit(0); //#Temporary, needs a mechanism for the system to select this

	(DataHandle.shader)->setUniformTextureUnit("colorTexture", 0);

	GLCALL(glDrawArrays(GL_TRIANGLES, 0, 36));
}
