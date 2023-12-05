#include "utils.h"
#include <GLFW/glfw3.h>

#include "vendor/stb_image/stb_image.h"

#include "Shader.h"
#include "Texture2D.h"
#include "geometrySetup.h"
#include "menu.h"
#include "Camera.h"

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

//void castDirectionalLight(glm::vec3 lightDir, )

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

	glEnable(GL_DEPTH_TEST);

	bool toggle = true;
	Material material;
	material.ambientColor = glm::vec3(1.0f, 0.5f, 0.31f);
	material.diffuseColor = glm::vec3(1.0f, 0.5f, 0.31f);
	material.specularColor = glm::vec3(0.5f, 0.5f, 0.5f);
	material.shininess = 32.0f;

	DirLight dirLight;

	dirLight.ambient = glm::vec3(0.1f, 0.1f, 0.1f);
	dirLight.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	dirLight.specular = glm::vec3(1.0f, 1.0f, 1.0f);
	dirLight.direction = glm::vec3(1.0f, -1.0f, 1.0f);

	PointLight pointLight[4];
	
	for (int x = 0; x < 4; x++)
	{
		(pointLight[x]).ambient = glm::vec3(0.1f, 0.1f, 0.1f);
		(pointLight[x]).diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
		(pointLight[x]).specular = glm::vec3(1.0f, 1.0f, 1.0f);

		(pointLight[x]).constant = 1.0f;
		(pointLight[x]).linear = 0.09f;
		(pointLight[x]).quadratic = 0.032f;
	}

	pointLight[0].position = glm::vec3(0.7f, 0.2f, 2.0f);
	pointLight[1].position = glm::vec3(2.3f, -3.3f, -4.0f);
	pointLight[2].position = glm::vec3(-4.0f, 2.0f, -12.0f);
	pointLight[3].position = glm::vec3(0.0f, 0.0f, -3.0f);

	std::cout <<glGetString(GL_VERSION) << "\n";

	//Setup Shaders:
	Shader shader_LightSource("C:/Code/Chalmers/myGraphicsCode/zenditeEngineV2/zenditeEngineV2/res/shaders/LightingShaders/vs_LightSource.glsl",
		"C:/Code/Chalmers/myGraphicsCode/zenditeEngineV2/zenditeEngineV2/res/shaders/LightingShaders/fs_LightSource.glsl");
	//shader_LightSource.bindProgram();

	Shader shader_blMaterial("C:/Code/Chalmers/myGraphicsCode/zenditeEngineV2/zenditeEngineV2/res/shaders/LightingShaders/vs_blMaterial.glsl",
		"C:/Code/Chalmers/myGraphicsCode/zenditeEngineV2/zenditeEngineV2/res/shaders/LightingShaders/fs_blMaterial.glsl");
	shader_blMaterial.bindProgram();

	Shader sh_multiLight("C:/Code/Chalmers/myGraphicsCode/zenditeEngineV2/zenditeEngineV2/res/shaders/multiLightShaders/vs_multiLight.glsl",
		"C:/Code/Chalmers/myGraphicsCode/zenditeEngineV2/zenditeEngineV2/res/shaders/multiLightShaders/fs_multiLight.glsl");
	//shader_DirLight.bindProgram();

	//Create our regular cube VAO
	unsigned int VAO_Cube;
	unsigned int VBO_Cube;
	GenerateCubeNoEBO(VAO_Cube, VBO_Cube);
	bindVao(VAO_Cube);

	//Create out light source cube: (uses the same VBO as the regular cube object, which improves data reuse)
	unsigned int VAO_LightCube;

	glGenVertexArrays(1, &VAO_LightCube);
	glBindVertexArray(VAO_LightCube);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_Cube);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	bindVao(VAO_Cube);

	//Setup Texture1
	Texture2D diffuseMap;
	diffuseMap.setupTexturePNG(0, "C:/Code/Chalmers/myGraphicsCode/zenditeEngineV2/zenditeEngineV2/res/textures/container2.png");

	//Setup Texture2
	Texture2D specularMap;
	specularMap.setupTexturePNG(1, "C:/Code/Chalmers/myGraphicsCode/zenditeEngineV2/zenditeEngineV2/res/textures/container2_specular.png");

	//IMGUI setup:
	imGuiSetup(window);

	int count = 0;
	bool rotation = false;
	float rotationSpeed = 30.0f;
	float specularStrength = 0.5f;
	int specularIntensity = 32;

	float angle = 1.0f;

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		count++;
		/* Render here */
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		if (rotation)
		{
			angle = angle + rotationSpeed * deltaTime;
		}

		glm::vec3 cubePositions(0.0f, 0.0f, 0.0f);

		glm::mat4 modelMat = glm::mat4(1.0f);
		modelMat = glm::translate(modelMat, cubePositions);
		glm::mat4 viewMat = camera.GetViewMatrix();
		glm::mat4 projMat = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		glm::vec3 lightCenter(0.0f, 1.0f, 0.0f);
		glm::vec4 lightRotation(1.5f, 0.0f, 0.0f, 0.0f);

		glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));

		lightRotation = rotationMatrix * lightRotation;

		glm::vec3 lightPos = glm::vec3(lightRotation) + lightCenter;

		//Draw LightCube:
		shader_LightSource.bindProgram();
		bindVao(VAO_LightCube);

		for (int i = 0; i < sizeof(pointLight) / sizeof(pointLight[0]); i++)
		{
			glm::mat4 translation = glm::translate(glm::mat4(1.0f), (pointLight[i]).position);
			glm::mat4 LC_modelMat = glm::mat4(1.0f) * translation;

			LC_modelMat = glm::translate(LC_modelMat, lightPos);
			LC_modelMat = glm::scale(LC_modelMat, glm::vec3(0.2f));

			shader_LightSource.setUniformMat4("viewMat", GL_FALSE, glm::value_ptr(viewMat));
			shader_LightSource.setUniformMat4("projMat", GL_FALSE, glm::value_ptr(projMat));
			shader_LightSource.setUniformMat4("modelMat", GL_FALSE, glm::value_ptr(LC_modelMat));

			GLCALL(glDrawArrays(GL_TRIANGLES, 0, 36));
		}

		//Draw Regular Cube
		bindVao(VAO_Cube);
		sh_multiLight.bindProgram();

		//shader_Transform.setUniformMat4("modelMat", GL_FALSE, glm::value_ptr(modelMat));
		sh_multiLight.setUniformMat4("modelMat", GL_FALSE, glm::value_ptr(modelMat));
		sh_multiLight.setUniformMat4("viewMat", GL_FALSE, glm::value_ptr(viewMat));
		sh_multiLight.setUniformMat4("projMat", GL_FALSE, glm::value_ptr(projMat));

		//Set fragment uniforms:

		glm::vec3 cameraPos = camera.getPosition();
		sh_multiLight.setUniform3fv("cameraWorldPos", cameraPos);

		sh_multiLight.setUniformFloat("material.shininess", material.shininess);
		sh_multiLight.setUniformTextureUnit("material.diffuse", diffuseMap.getTexUnit());
		sh_multiLight.setUniformTextureUnit("material.specular", specularMap.getTexUnit());
		
		sh_multiLight.setUniform3fv("dirLight.direction", dirLight.direction);
		
		sh_multiLight.setUniform3fv("dirLight.ambient", dirLight.ambient);
		sh_multiLight.setUniform3fv("dirLight.diffuse", dirLight.diffuse);
		sh_multiLight.setUniform3fv("dirLight.specular", dirLight.specular);

		for (int i = 0; i < sizeof(pointLight) / sizeof(pointLight[0]); i++)
		{
			std::string pointLightStr = "pointLight[" + std::to_string(i) + "]";

			sh_multiLight.setUniform3fv((pointLightStr + ".position"), (pointLight[i]).position);

			sh_multiLight.setUniform3fv((pointLightStr + ".ambient"), (pointLight[i]).ambient);
			sh_multiLight.setUniform3fv((pointLightStr + ".diffuse"), (pointLight[i]).diffuse);
			sh_multiLight.setUniform3fv((pointLightStr + ".specular"), (pointLight[i]).specular);

			sh_multiLight.setUniformFloat((pointLightStr + ".linear"), (pointLight[i]).linear);
			sh_multiLight.setUniformFloat((pointLightStr + ".quadratic"), (pointLight[i]).quadratic);
			sh_multiLight.setUniformFloat((pointLightStr + ".constant"), (pointLight[i]).constant);

		}

		GLCALL(glDrawArrays(GL_TRIANGLES, 0, 36));

		//Create IMGUI menu:
		ImGui_ImplGlfwGL3_NewFrame();

		ImGui::Begin("Test");

		ImGui::Text("Dear ImGui, %s", ImGui::GetVersion());
		ImGui::Separator();
		if (ImGui::Button("Recompile Shaders")) 
		{
			shader_LightSource.recompile();
			shader_blMaterial.recompile();
		}
		ImGui::NewLine();
		//diffuseIntensity
		ImGui::InputFloat3("diffuse Color", &(material.diffuseColor)[0]);
		ImGui::InputFloat3("specular Color", &(material.specularColor)[0]);
		ImGui::NewLine();
		//ImGui::InputFloat3("ambient light", &(light.ambient)[0]);
		//ImGui::InputFloat3("diffuse light", &(light.diffuse)[0]);
		//ImGui::InputFloat3("specular light", &(light.specular)[0]);
		ImGui::NewLine();
		ImGui::SliderFloat("Specular Shininess", &material.shininess, 1.0, 64.0);
		if (ImGui::Button("Toggle Rotation"))
		{
			if (rotation)
			{
				rotation = false;
			}
			else
			{
				rotation = true;
			}
		}
		ImGui::SliderFloat("Rotation Speed per frame", &rotationSpeed, 10.0f, 180.0f);
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
		//ImGui::InputFloat("linear Light Val", &(light.linear));
		//ImGui::InputFloat("quadratic Light Val", &(light.quadratic));

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