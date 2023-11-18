#include "utils.h"
#include <GLFW/glfw3.h>

#include "vendor/stb_image/stb_image.h"

#include "Shader.h"
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

	//Textures:
	/*
	Note: To use multiple textures you need to store textures in TEXTURE UNITS. openGl has about 16 or 32 by default,
	once you have created and bound a texture with a texture handle ID, you can store this in a texture unit.

	You need to store the texture in these units for glsl to retrieve them via a uniform. Having multiple texture units allows
	glsl to work with multiple textures at a time in a single draw call.
	*/

	//Wall tex location: C:/Code/Chalmers/myGraphicsCode/zenditeEngineV2/zenditeEngineV2/res/textures/wall.jpg
	int width, height, nrChannels;
	unsigned char* data = stbi_load("C:/Code/Chalmers/myGraphicsCode/zenditeEngineV2/zenditeEngineV2/res/textures", &width, &height, &nrChannels, 0);

	unsigned int tex_wall;
	glGenTextures(1, &tex_wall);

	glBindTexture(GL_TEXTURE_2D, tex_wall);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_image_free(data);

	//Now lets bind our texture to texture unit 0:
	glActiveTexture(GL_TEXTURE0); // Activate texture unit 0
	glBindTexture(GL_TEXTURE_2D, tex_wall); //Yep, same as before. When you bind a texture handle it also auto binds to the most recently activated texture unit

	shader_2.setUniformTextureUnit("colorTexture", 0);

	//IMGUI setup:
	imGuiSetup(window);

	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader_2.bindProgram();
		//float timeValue = glfwGetTime();
		//float greenValue = sin(timeValue) / 2.0f + 0.5f;
		//shader_1.setUniform4f("ourColor", 0.1f, greenValue, 0.1f, 1.0f);

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