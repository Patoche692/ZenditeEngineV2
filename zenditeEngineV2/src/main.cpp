// #define GLEW_STATIC //Can be done here or in the VS properties preprocessor definitions

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>

#include <iostream>

//Basic Error Checking
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCALL(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << ") \nFunction: "
			<< function << "\nFile: "
			<< file << "\nLine: "
			<< line << "\n"
			<< std::endl;

		return false;
	}

	return true;
}

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

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
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
	unsigned int vShader = glCreateShader(GL_VERTEX_SHADER);


	
	//IMGUI setup:
	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(window, true);
	ImGui::StyleColorsDark();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

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

		//Draw Call HERE
		

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}