#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "ECS/Components.h"

#include <cstdlib>


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

void genMenu_1(c_Transform& posData, c_Texture& texData, c_Modified& modified, short int containerTexUnit, unsigned short int rockySurfaceTexUnit)
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
    	if (ImGui::SliderFloat3("My Vec3 Slider", &posData.pos[0], -5.0f, 5.0f));
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



    if (ImGui::Begin("Entities", nullptr))
    {
        ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);

        int selected = 0;
        {
            ImGui::BeginChild("left pane", ImVec2(150, 0), ImGuiChildFlags_Border | ImGuiChildFlags_ResizeX);
            for (int i = 0; i < 100; i++)
            {
                // FIXME: Good candidate to use ImGuiSelectableFlags_SelectOnNav
                char label[128];
                sprintf_s(label, "GameObject %d", i);
                if (ImGui::Selectable(label, selected == i))
                    selected = i;
            }
            ImGui::EndChild();

        }
        ImGui::SameLine();

        // Right
        {
            ImGui::BeginGroup();
            ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
            ImGui::Text("GameObject: %d", selected);
            ImGui::Separator();
            if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
            {
                if (ImGui::BeginTabItem("Description"))
                {
                    if (ImGui::SliderFloat3("Position XYZ", &posData.pos[0], -5.0f, 5.0f));  //Position
                    {
                        // The slider was used; myVec3 has been updated.
                        // You can handle the change here if needed.
                    }

                    if (ImGui::SliderFloat3("Scale XYZ", &posData.scale[0], -5.0f, 5.0f));  //Scale
                    {
                        // The slider was used; myVec3 has been updated.
                        // You can handle the change here if needed.
                    }
                    ImGui::EndTabItem();

                    if (ImGui::Button("Texture")) {
                        
                        // We were trying to add custom texture here via openLocalRepository() but it didn't work

                    }

                    static int selected_fish = -1;
                    const char* names[] = { "Wooden planks", "Rock"};
                    static bool toggles[] = { true, false};


                    // Simple selection popup (if you want to show the current selection inside the Button itself,
                    // you may want to build a string using the "###" operator to preserve a constant ID with a variable label)
                    if (ImGui::Button("Select texture"))
                        ImGui::OpenPopup("my_select_popup");
                    ImGui::SameLine();
                    ImGui::TextUnformatted(selected_fish == -1 ? "<None>" : names[selected_fish]);
                    if (ImGui::BeginPopup("my_select_popup"))
                    {
                        ImGui::SeparatorText("Default textures");
                        for (int i = 0; i < IM_ARRAYSIZE(names); i++)
                            if (ImGui::Selectable(names[i]))
                                selected_fish = i;
                        ImGui::EndPopup();
                    }
                }
                if (ImGui::BeginTabItem("Details"))
                {
                    ImGui::Text("ID: 0123456789");
                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }
            ImGui::EndChild();
            if (ImGui::Button("Revert")) {}
            ImGui::SameLine();
            if (ImGui::Button("Save")) {}
            ImGui::EndGroup();
        }
        ImGui::End();
    }
	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

void openLocalRepository(const std::string& repositoryPath) 
{
    
#ifdef _WIN32
    std::string command = "explorer \"" + repositoryPath + "\"";

#endif
    system(command.c_str());

}

