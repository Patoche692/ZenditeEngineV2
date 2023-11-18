// #define GLEW_STATIC //Can be done here or in the VS properties preprocessor definitions

#include "utils.h"
#include <GLFW/glfw3.h>

#include "Shader.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

int main(void)
{
	GLFWwindow* window;

	if (!glfwInit()) {
		return -1;
	}

	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	//Sets the frames to be drawn equal my machines refresh rate (which is likely 60Hz - screen refreshes 60 times a second)
	//Ideally our framerate should be equal to the screen Hz refresh rate. (VSYNC)
	glfwSwapInterval(1);

	//GLEW init must only be called after we have a valid openGL context (which is created above using GLFW: glfwMakeContextCurrent(window))
	if (glewInit() != GLEW_OK)
	{
		std::cout << "I'm am not GLEW_OK, I'm GLEW_SAD :(\n";
	}

	std::cout << glGetString(GL_VERSION);

	//Setup Shaders:
	Shader shader_1(vertexShaderSource, fragmentShaderSource);

	//Object Data Setup:
	float vertices[] = {
		 0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};

	unsigned int VAO_Square;
	unsigned int VBO_Square;
	unsigned int EBO_Square;

	GLCALL(glGenVertexArrays(1, &VAO_Square));
	GLCALL(glBindVertexArray(VAO_Square));

	GLCALL(glGenBuffers(1, &VBO_Square));
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, VBO_Square));
	GLCALL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

	GLCALL(glGenBuffers(1, &EBO_Square));
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_Square));
	GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

	GLCALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, (void*)0));
	GLCALL(glEnableVertexAttribArray(0));

	GLCALL(glBindVertexArray(0));

	GLCALL(glUseProgram(shader_1.getShaderHandle()));

	//IMGUI setup:
	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(window, true);
	ImGui::StyleColorsDark();

	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Draw Call HERE
		GLCALL(glBindVertexArray(VAO_Square));
		GLCALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

		ImGui_ImplGlfwGL3_NewFrame();

		ImGui::Begin("Test");

		//#Write_IMGUI_things_here
		ImGui::Text("Dear ImGui, %s", ImGui::GetVersion());
		ImGui::Separator();
		ImGui::Text("By Omar Cornut and all dear imgui contributors.");
		ImGui::Text("Dear ImGui is licensed under the MIT License, see LICENSE for more information.");

		ImGui::End();
		

		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}