#pragma once
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>
#include "ECS/Components.h"

void imGuiSetup(GLFWwindow* window)
{
	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(window, true);
	ImGui::StyleColorsDark();
}

void genMenu_1(c_Transform& posData)
{
	//Create IMGUI menu:
	ImGui_ImplGlfwGL3_NewFrame();
	//ImGui::NewFrame();

	ImGui::Begin("GUI");
	// --------------------------------------------------------------


	ImGui::Separator();

	if (ImGui::SliderFloat3("My Vec3 Slider", &posData.pos[0], -5.0f, 5.0f));
	{
		// The slider was used; myVec3 has been updated.
		// You can handle the change here if needed.
	}

	ImGui::NewLine();


	// --------------------------------------------------------------
	ImGui::End();
	ImGui::Render();
	ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

}

