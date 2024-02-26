#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "ECS/Components.h"

int selectedOption = 0; // You can assign default selection here, 0 for Option A, 1 for Option B, etc.
int lastSelectedOption = selectedOption;

void imGuiSetup(GLFWwindow* window)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130"); // Use GLSL version 130

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
}

void genMenu_1(std::vector<Entity>& entities, Coordinator& COORD, short int containerTexUnit, unsigned short int rockySurfaceTexUnit)
{
	auto& posData = COORD.GetComponentDataFromEntity<c_Transform>(entities[0]);
	auto& texData = COORD.GetComponentDataFromEntity<c_Texture>(entities[0]);
	auto& modifiedData = COORD.GetComponentDataFromEntity<c_Modified>(entities[0]);

	auto& infoData = COORD.GetComponentDataFromEntity<c_EntityInfo>(entities[0]);

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Here, you can start using ImGui to create interfaces
	ImGui::Begin("Hello, world!");
	ImGui::Text("Name of Entity 0 = %s", infoData.name.c_str());

	ImGui::Separator();

	if (ImGui::SliderFloat3("My Vec3 Slider", &posData.pos[0], -5.0f, 5.0f));
	{
		// The slider was used; myVec3 has been updated.
		// You can handle the change here if needed.
	}

	if (ImGui::SliderFloat3("AABB scale", &aabb.scale[0], -5.0f, 5.0f));
	{
		// The slider was used; myVec3 has been updated.
		// You can handle the change here if needed.
	}

	ImGui::NewLine();
	
	ImGui::Text("Choose a Texture:");
	if (ImGui::RadioButton("Wooden Box", selectedOption == 0)) 
	{
		selectedOption = 0;
		if (selectedOption != lastSelectedOption)
		{
			
			texData.texUnit = containerTexUnit;
			modified.isModifed = true;
			lastSelectedOption = selectedOption;
			std::cout << selectedOption << std::endl;
		}
	}
	if (ImGui::RadioButton("Rocky Surface", selectedOption == 1)) 
	{
		selectedOption = 1;
		if (selectedOption != lastSelectedOption)
		{
			
			texData.texUnit = rockySurfaceTexUnit;
			modified.isModifed = true;
			lastSelectedOption = selectedOption;
			std::cout << selectedOption << std::endl;
		}
	}

	ImGui::NewLine();
	ImGui::NewLine();

	//Radio Button Testing:
	/*
	if (ImGui::RadioButton("Label", &variable, value)) {
		// Action to take when this radio button is selected
	}
	*/

	ImGui::End();


	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());



}

