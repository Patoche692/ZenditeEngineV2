#include "utils.h"
#include <GLFW/glfw3.h>

#include "vendor/stb_image/stb_image.h"

#include "Shader.h"
#include "Texture2D.h"
#include "geometrySetup.h"
#include "menu.h"

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main(void)
{
	GLFWwindow* window;
	if (!glfwInit()) {
		return -1;
	}
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window){
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	if (glewInit() != GLEW_OK)
	{
		std::cout << "I'm am not GLEW_OK, I'm GLEW_SAD :(\n";
	}

	std::cout <<glGetString(GL_VERSION) << "\n";

	//Setup Shaders:
	Shader shader_1("C:/Code/Chalmers/myGraphicsCode/zenditeEngineV2/zenditeEngineV2/res/shaders/vs_Basic.glsl", 
		"C:/Code/Chalmers/myGraphicsCode/zenditeEngineV2/zenditeEngineV2/res/shaders/fs_Basic.glsl");
	shader_1.bindProgram();
	shader_1.setUniform4f("ourColor", 0.0f, 0.5f, 0.0f, 1.0f);

	Shader shader_2("C:/Code/Chalmers/myGraphicsCode/zenditeEngineV2/zenditeEngineV2/res/shaders/vs_Texture.glsl",
		"C:/Code/Chalmers/myGraphicsCode/zenditeEngineV2/zenditeEngineV2/res/shaders/fs_Texture.glsl");
	shader_2.bindProgram();

	unsigned int VAO_Square;
	unsigned int VBO_Square;
	unsigned int EBO_Square;
	GenerateBasicSquareVAO(VAO_Square, VBO_Square, EBO_Square);

	unsigned int VAO_TexTri;
	unsigned int VBO_TexTri;
	unsigned int EBO_TexTri;
	GenerateTexTriVAO(VAO_TexTri, VBO_TexTri, EBO_TexTri);
	GLCALL(glBindVertexArray(VAO_TexTri));

	//Setup Texture1
	Texture2D wallTexture;
	wallTexture.setupTextureJPG(0, "C:/Code/Chalmers/myGraphicsCode/zenditeEngineV2/zenditeEngineV2/res/textures/wall.jpg");

	shader_2.setUniformTextureUnit("colorTexture", wallTexture.getTexUnit());

	Texture2D faceTexture;
	faceTexture.setupTexturePNG(1, "C:/Code/Chalmers/myGraphicsCode/zenditeEngineV2/zenditeEngineV2/res/textures/awesomeface.png");

	//IMGUI setup:
	imGuiSetup(window);

	int count = 0;

	while (!glfwWindowShouldClose(window))
	{
		count++;
		/* Render here */
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader_2.bindProgram();
		float timeValue = glfwGetTime();
		//float greenValue = sin(timeValue) / 2.0f + 0.5f;
		//shader_1.setUniform4f("ourColor", 0.1f, greenValue, 0.1f, 1.0f);

		if (count > 360)
		{
			shader_2.setUniformTextureUnit("colorTexture", wallTexture.getTexUnit());
		}
		else
		{
			shader_2.setUniformTextureUnit("colorTexture", faceTexture.getTexUnit());
		}
		//Draw Call HERE
		GLCALL(glBindVertexArray(VAO_TexTri));
		
		GLCALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

		//Create IMGUI menu:
		genMenu_1();

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}