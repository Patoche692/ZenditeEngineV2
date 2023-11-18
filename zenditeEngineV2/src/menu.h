#pragma once
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>

void imGuiSetup(GLFWwindow* window)
{
	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(window, true);
	ImGui::StyleColorsDark();
}

void genMenu_1()
{
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
}