#pragma once
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>

void imGuiSetup(GLFWwindow* window)
{
	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(window, true);
	ImGui::StyleColorsDark();
}

void genMenu_1(bool& toggle, bool& wireframe, bool& rotation)
{
	//Create IMGUI menu:
	ImGui_ImplGlfwGL3_NewFrame();
	//ImGui::NewFrame();

	ImGui::Begin("GUI");
	ImGui::Separator();
	if (ImGui::Button("Toggle Wireframe"))
	{
		if (wireframe == false)
		{
			wireframe = true;
		}
		else
		{
			wireframe = false;
		}
	}
	ImGui::NewLine();
	if (ImGui::Button("Toggle Rotation"))
	{
		if (rotation)
		{
			//glDisable(GL_DEPTH_TEST);
			rotation = false;
		}
		else
		{
			//glEnable(GL_DEPTH_TEST);
			rotation = true;
		}
	}
	ImGui::NewLine();

	ImGui::End();
	ImGui::Render();
	ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

}