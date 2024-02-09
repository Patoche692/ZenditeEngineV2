#include "../utils.h"
#include "OpenGl_Renderer.h"
#include "../ECS/ECSCoordinator.h"
#include "../Shader.h"
#include "../geometrySetup.h"

void OpenGL_Renderer::Render(const R_DataHandle& DataHandle, const transform& trans)
{
	(DataHandle.shader)->bindProgram();
	bindVao(DataHandle.VAO);
	glm::mat4 cubeProjection = glm::perspective(glm::radians(cam.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 cubeView = cam.GetViewMatrix();
	(DataHandle.shader)->setUniformMat4("projection", GL_FALSE, glm::value_ptr(cubeProjection));
	(DataHandle.shader)->setUniformMat4("view", GL_FALSE, glm::value_ptr(cubeView));

	glm::mat4 cubeModel = glm::mat4(1.0f);
	cubeModel = glm::translate(cubeModel, glm::vec3(1.5f, 0.0f, -1.2f));
	cubeModel = glm::scale(cubeModel, glm::vec3(1.0f, 1.0f, 1.0f));
	(DataHandle.shader)->setUniformMat4("model", GL_FALSE, glm::value_ptr(cubeModel));

	//cubeTex.changeTexUnit(0);

	//sh_basicWithTex.setUniformTextureUnit("colorTexture", 0);

	GLCALL(glDrawArrays(GL_TRIANGLES, 0, 36));
}
