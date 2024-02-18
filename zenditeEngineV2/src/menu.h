#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "ECS/Components.h"

void imGuiSetup(GLFWwindow* window)
{
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui::StyleColorsDark();
}

void genMenu_1(c_Transform& posData)
{

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Here, you can start using ImGui to create interfaces
	ImGui::Begin("Hello, world!");
	ImGui::Text("This is some useful text.");

	ImGui::Separator();

	if (ImGui::SliderFloat3("My Vec3 Slider", &posData.pos[0], -5.0f, 5.0f));
	{
		// The slider was used; myVec3 has been updated.
		// You can handle the change here if needed.
	}

	ImGui::NewLine();

	ImGui::End();

	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());



}

