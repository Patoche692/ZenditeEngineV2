#include "utils.h"
#include <GLFW/glfw3.h>

#include "vendor/stb_image/stb_image.h"
#include "assimp/Importer.hpp"

#include "Shader.h"
#include "Texture2D.h"
#include "geometrySetup.h"
#include "menu.h"
#include "Camera.h"

#include "Helper/Model.h"

#include <filesystem>

#include "Coordinator.h"
#include "ECS/Components.h"

//ECS implementation ver 1.0

namespace fs = std::filesystem;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

//GUI Menu Toggles
bool toggle = true;
bool wireframe = false;
bool rotation = false;


//Coordinator:

int main(void)
{
	GLFWwindow* window;
	if (!glfwInit()) {
		return -1;
	}
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Zen", NULL, NULL);
	if (!window){
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "I'm am not GLEW_OK, I'm GLEW_SAD :(\n";
	}


	stbi_set_flip_vertically_on_load(true); //#### THIS NEEDS TO BE ACTIVE ### or else image texture will be upside down.
	//std::cout << "Current working directory: " << fs::current_path() << std::endl;
	glEnable(GL_DEPTH_TEST);

	//#Removed_3: 78 - 88

	std::cout << glGetString(GL_VERSION) << "\n";

	//#Removed_2: 92 - 184

	//IMGUI setup:
	imGuiSetup(window);

	//Check the number of texture units we can have on the GPU
	GLint maxTextureUnits;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
	std::cout << "Maximum texture units available: " << maxTextureUnits << std::endl;

	//Testing ECS:
	Coordinator COORD("opengl");
	COORD.RegisterComponents();
	COORD.RegisterSystems();
	COORD.SetUpSystemBitsets();

	Shader sh_basicWithTex("res/shaders/BasicShaders/vs_cubeWnormANDtex.glsl",
		"res/shaders/BasicShaders/fs_cubeWnormANDtex.glsl");

	std::vector<Entity> entities(MAX_ENTITIES);

	entities[0] = COORD.CreateEntity();

	c_Transform tr_0;
	tr_0.pos = glm::vec3(0.0f,0.0f,0.0f);
	tr_0.scale = glm::vec3(1.0f, 1.0f, 1.0f);

	COORD.AddComponentToEntity<c_Transform>(entities[0], tr_0);


	//Old Rendering:

	/*
	Shader sh_basicWithTex("res/shaders/BasicShaders/vs_cubeWnormANDtex.glsl",
		"res/shaders/BasicShaders/fs_cubeWnormANDtex.glsl");
	*/

	unsigned int CubeVAO;
	unsigned int CubeVBO;

	GenerateCubeNoEBO(CubeVAO, CubeVBO);
	Texture2D cubeTex("diffuse");
	cubeTex.setupTexturePNG(0, "res/textures/container2.png");


	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		/* Render here */
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// ----------------------------------------------------------------------
		sh_basicWithTex.bindProgram();
		bindVao(CubeVAO);
		glm::mat4 cubeProjection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 cubeView = camera.GetViewMatrix();
		sh_basicWithTex.setUniformMat4("projection", GL_FALSE, glm::value_ptr(cubeProjection));
		sh_basicWithTex.setUniformMat4("view", GL_FALSE, glm::value_ptr(cubeView));

		glm::mat4 cubeModel = glm::mat4(1.0f);
		cubeModel = glm::translate(cubeModel, glm::vec3(1.5f, 0.0f, -1.2f));
		cubeModel = glm::scale(cubeModel, glm::vec3(1.0f, 1.0f, 1.0f));
		sh_basicWithTex.setUniformMat4("model", GL_FALSE, glm::value_ptr(cubeModel));

		cubeTex.changeTexUnit(0);

		sh_basicWithTex.setUniformTextureUnit("colorTexture", 0);

		GLCALL(glDrawArrays(GL_TRIANGLES, 0, 36));


		/* End Rendering Code */ // ----------------------------------------------

		//#Removed_1: 206 - 314

		if (wireframe){
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}


		genMenu_1(toggle, wireframe, rotation);


		glfwPollEvents();

		glfwSwapBuffers(window);

		
	}

	glfwTerminate();
	return 0;
}


void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);

	if(glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		glfwSetCursorPosCallback(window, nullptr);
	}
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPosCallback(window, mouse_callback);
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
	
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}