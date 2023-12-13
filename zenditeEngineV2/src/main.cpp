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

struct Vert
{
	//Position
	glm::vec3 pos;

	//TexCoords
	glm::vec2 texCord;

};

struct Material
{
	glm::vec3 ambientColor;
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
	float shininess; //AKA: specularStrength
};

struct PointLight
{
	glm::vec3 position;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct DirLight
{
	glm::vec3 direction;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

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

int main(void)
{
	GLFWwindow* window;
	if (!glfwInit()) {
		return -1;
	}
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello GRAPHICS", NULL, NULL);
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


	glEnable(GL_DEPTH_TEST);

	std::cout <<glGetString(GL_VERSION) << "\n";

	Shader sh_basicWithTex("C:/Code/Chalmers/myGraphicsCode/zenditeEngineV2/zenditeEngineV2/res/shaders/BasicShaders/vs_cubeWnormANDtex.glsl",
		"C:/Code/Chalmers/myGraphicsCode/zenditeEngineV2/zenditeEngineV2/res/shaders/BasicShaders/fs_cubeWnormANDtex.glsl");

	Shader sh_modelLoading("C:/Code/Chalmers/myGraphicsCode/zenditeEngineV2/zenditeEngineV2/res/shaders/modelLoading/vs_model_loading.glsl",
		"C:/Code/Chalmers/myGraphicsCode/zenditeEngineV2/zenditeEngineV2/res/shaders/modelLoading/fs_model_loading.glsl");

	//HeightMap SetUp
	// ----------------------------------

	Shader sh_HeightMap("C:/Code/Chalmers/myGraphicsCode/zenditeEngineV2/zenditeEngineV2/res/shaders/HeightMap/vs_basicHeightMap.glsl",
		"C:/Code/Chalmers/myGraphicsCode/zenditeEngineV2/zenditeEngineV2/res/shaders/HeightMap/fs_basicHeightMap.glsl");

	//Get height map texture data:
	int hmWidth;
	int hmHeight;
	int hmNrChannels;

	//"C:/Code/Chalmers/myGraphicsCode/zenditeEngineV2/zenditeEngineV2/res/textures/heightmap.png"
	unsigned char* hmTexData = stbi_load("C:/Code/Chalmers/myGraphicsCode/zenditeEngineV2/zenditeEngineV2/res/textures/heightmap.png", &hmWidth, &hmHeight, &hmNrChannels, 0);

	if (!hmTexData)
	{
		std::cout << "\n --- Failed to load texture --- \n";
		ASSERT(false);
	}

	//Set up height map VAO, VBO and EBO (if needed)
	unsigned int heightMapVAO;
	unsigned int heightMapVBO;
	unsigned int heightMapEBO;

	std::vector<Vert> hmVerts;
	std::vector<unsigned int> hmIndices;

	//Load in vertex data to the heightMapVBO Data (using the hmTexData for the y axis Data):

	float yScale = 64.0f / 256.0f; 
	float yShift = 16.0f;

	float x_texPoint = 1.0f / hmWidth;
	float y_texPoint = 1.0f / hmHeight;

	for(unsigned int z = 0; z < hmHeight; z++)
	{
		for(unsigned int x = 0; x < hmWidth; x++)
		{
			Vert vert;
			unsigned char* texel = hmTexData + (x + hmWidth * z) * hmNrChannels;
			unsigned char y = texel[0];

			glm::vec3 vec;
			vec.x = x;
			vec.y = (int)y * yScale - yShift;
			vec.z = z;

			vert.pos = vec;

			glm::vec2 texCoord;
			texCoord.x = x * x_texPoint;
			texCoord.y = z * y_texPoint;

			vert.texCord = texCoord;

			hmVerts.push_back(vert);
		}
	}

	//Fill Height map indices data:
	for(unsigned int i = 0; i < (hmHeight-1); i++)
	{
		for(unsigned int ii = 0; ii < (hmWidth-1)*6; ii = ii + 6)
		{
			hmIndices.push_back(i* hmWidth + ii/6);
			hmIndices.push_back((i + 1) * hmWidth + ii/6);
			hmIndices.push_back(((i + 1) * hmWidth + ii/6) + 1);
			
			hmIndices.push_back(i* hmWidth + ii/6);
			hmIndices.push_back(((i + 1)* hmWidth + ii/6) + 1);
			hmIndices.push_back((i* hmWidth + ii/6) + 1);
		}
	}

	GLCALL(glGenVertexArrays(1, &heightMapVAO));
	GLCALL(glGenBuffers(1, &heightMapVBO));
	GLCALL(glGenBuffers(1, &heightMapEBO));

	GLCALL(glBindVertexArray(heightMapVAO));
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, heightMapVBO));

	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, heightMapVBO));
	GLCALL(glBufferData(GL_ARRAY_BUFFER, hmVerts.size() * sizeof(Vert), &hmVerts[0], GL_STATIC_DRAW));

	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, heightMapEBO));
	GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, hmIndices.size() * sizeof(unsigned int), &hmIndices[0], GL_STATIC_DRAW));

	GLCALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vert), (void*)0));
	GLCALL(glEnableVertexAttribArray(0));

	GLCALL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vert), (void*)offsetof(Vert, texCord)));
	GLCALL(glEnableVertexAttribArray(1));


	//Texture2D heightMapTex("diffuse");
	//heightMapTex.setupHeightMapTexturePNG(1, "C:/Code/Chalmers/myGraphicsCode/zenditeEngineV2/zenditeEngineV2/res/textures/mt_fuji.png");

	// ----------------------------------
	//End HeightMap SetUp
	

	unsigned int CubeVAO;
	unsigned int CubeVBO;
	
	GenerateCubeNoEBO(CubeVAO, CubeVBO);
	Texture2D cubeTex("diffuse");
	cubeTex.setupTexturePNG(0, "C:/Code/Chalmers/myGraphicsCode/zenditeEngineV2/zenditeEngineV2/res/textures/container2.png");

	//Model ourModel("C:/Code/Chalmers/myGraphicsCode/zenditeEngineV2/zenditeEngineV2/res/models/backpack/backpack.obj", sh_modelLoading);

	//IMGUI setup:
	imGuiSetup(window);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		/* Render here */
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//HeightMapRendering:
		sh_HeightMap.bindProgram();
		GLCALL(glBindVertexArray(heightMapVAO));

		glm::mat4 hmProjection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 hmView = camera.GetViewMatrix();
		sh_HeightMap.setUniformMat4("projection", GL_FALSE, glm::value_ptr(hmProjection));
		sh_HeightMap.setUniformMat4("view", GL_FALSE, glm::value_ptr(hmView));

		glm::mat4 hmModel = glm::mat4(1.0f);
		hmModel = glm::translate(hmModel, glm::vec3(0.0f, -3.0f, -1.0f));
		hmModel = glm::scale(hmModel, glm::vec3(0.01f, 0.01f, 0.01f));
		sh_HeightMap.setUniformMat4("model", GL_FALSE, glm::value_ptr(hmModel));

		//hmTex.changeTexUnit(0);

		//sh_HeightMap.setUniformTextureUnit("colorTexture", 0);
		GLCALL(glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(hmIndices.size()), GL_UNSIGNED_INT, 0));

		//Old Rendering:
		sh_modelLoading.bindProgram();
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
		
		//Model Rendering:
		sh_modelLoading.bindProgram();

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		sh_modelLoading.setUniformMat4("projection", GL_FALSE, glm::value_ptr(projection));
		sh_modelLoading.setUniformMat4("view", GL_FALSE, glm::value_ptr(view));

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		sh_modelLoading.setUniformMat4("model", GL_FALSE, glm::value_ptr(model));
		//ourModel.Draw(sh_modelLoading);

		if(wireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		//Create IMGUI menu:
		ImGui_ImplGlfwGL3_NewFrame();

		ImGui::Begin("GUI");
		ImGui::Separator();
		if (ImGui::Button("Toggle Wireframe"))
		{
			if(wireframe == false)
			{
				wireframe = true;
			}
			else
			{
				wireframe = false;
			}
		}
		ImGui::NewLine();
		if(ImGui::Button("Toggle Depth Test"))
		{
			if (toggle)
			{
				glDisable(GL_DEPTH_TEST);
				toggle = false;
			}
			else
			{
				glEnable(GL_DEPTH_TEST);
				toggle = true;
			}
		}
		ImGui::NewLine();

		ImGui::End();
		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);

		glfwPollEvents();
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