#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Coordinator.h"
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

void genMenu_1(std::vector<Entity>& entities, Coordinator& COORD, short int containerTexUnit, unsigned short int rockySurfaceTexUnit, unsigned short int grassTexUnit, unsigned short int waterTexUnit, unsigned short int lavaTexUnit)
{

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Here, you can start using ImGui to create interfaces

	if (ImGui::Begin("Entities", nullptr))
	{

		ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);

		static int selected = 0;
		{
			ImGui::BeginChild("left pane", ImVec2(150, 0), ImGuiChildFlags_Border | ImGuiChildFlags_ResizeX);
			for (int i = 0; i < entities.size(); i++)
			{
				// FIXME: Good candidate to use ImGuiSelectableFlags_SelectOnNav
				char label[50];
				std::string name = COORD.GetComponentDataFromEntity<c_EntityInfo>(entities[i]).name;

				sprintf_s(label, " %s", name.c_str());
				if (ImGui::Selectable(label, selected == i))
					selected = i;
			}
			ImGui::EndChild();

		}
		ImGui::SameLine();

		// Right
		{

			auto& modified = COORD.GetComponentDataFromEntity<c_Modified>(entities[selected]);
			auto& infoData = COORD.GetComponentDataFromEntity<c_EntityInfo>(entities[selected]);

			//auto& aabb = COORD.GetComponentDataFromEntity<c_AABB>(entities[selected]);

			ImGui::BeginGroup();
			ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
			ImGui::Text("%s", infoData.name.c_str());
			ImGui::Separator();
			if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))

			{
				if (ImGui::BeginTabItem("Description"))
				{
					short int bitSetPos = COORD.GetComponentBitsetPos<c_Transform>();
					std::bitset<32> transBitset; // Create a bitset of size 32
					transBitset.set(bitSetPos);

					std::bitset<32> entitySig = COORD.GetEntitySignature(entities[selected]);

					if ((entitySig & transBitset) == transBitset) // If this entity has a texture component
					{
						ImGui::SeparatorText("Transform:");

						ImGui::Separator;

						auto& posData = COORD.GetComponentDataFromEntity<c_Transform>(entities[selected]);

						glm::mat4 modelMat = posData.modelMat[0];

						glm::vec3 pos;
						glm::vec3 scale;
						pos.x = modelMat[3][0];
						pos.y = modelMat[3][1];
						pos.z = modelMat[3][2];

						scale.x = glm::length(glm::vec3(modelMat[0]));
						scale.y = glm::length(glm::vec3(modelMat[1]));
						scale.z = glm::length(glm::vec3(modelMat[2]));

						modelMat[0] = modelMat[0] / scale.x;
						modelMat[1] = modelMat[1] / scale.y;
						modelMat[2] = modelMat[2] / scale.z;

						glm::vec3 angles;
						glm::quat q = glm::quat_cast(modelMat);
						angles = glm::eulerAngles(q);
						angles = glm::degrees(angles);

						if (ImGui::InputFloat3("Position XYZ", &pos[0]))  //Position
						{
							float angleXRadians = glm::radians(angles.x);
							float angleYRadians = glm::radians(angles.y);
							float angleZRadians = glm::radians(angles.z);

							glm::mat4 MM(1.0f);
							MM = glm::translate(MM, pos);
							MM = glm::rotate(MM, angleZRadians, glm::vec3(0, 0, 1)); //  Z
							MM = glm::rotate(MM, angleYRadians, glm::vec3(0, 1, 0)); //  Y
							MM = glm::rotate(MM, angleXRadians, glm::vec3(1, 0, 0)); //  X
							MM = glm::scale(MM, scale);

							posData.modelMat[0] = MM;

						}

						if (ImGui::InputFloat3("Rotation XYZ", &angles[0]))  //Rotation
						{
							float angleXRadians = glm::radians(angles.x);
							float angleYRadians = glm::radians(angles.y);
							float angleZRadians = glm::radians(angles.z);

							glm::mat4 MM(1.0f);
							MM = glm::translate(MM, pos);
							MM = glm::rotate(MM, angleZRadians, glm::vec3(0, 0, 1)); //  Z
							MM = glm::rotate(MM, angleYRadians, glm::vec3(0, 1, 0)); //  Y
							MM = glm::rotate(MM, angleXRadians, glm::vec3(1, 0, 0)); //  X
							MM = glm::scale(MM, scale);

							posData.modelMat[0] = MM;
						}

						if (ImGui::InputFloat3("Scale XYZ", &scale[0]))  //Scale
						{
							float angleXRadians = glm::radians(angles.x);
							float angleYRadians = glm::radians(angles.y);
							float angleZRadians = glm::radians(angles.z);

							glm::mat4 MM(1.0f);
							MM = glm::translate(MM, pos);
							MM = glm::rotate(MM, angleZRadians, glm::vec3(0, 0, 1)); //  Z
							MM = glm::rotate(MM, angleYRadians, glm::vec3(0, 1, 0)); //  Y
							MM = glm::rotate(MM, angleXRadians, glm::vec3(1, 0, 0)); //  X
							MM = glm::scale(MM, scale);

							posData.modelMat[0] = MM;
						}

					}
					else
					{
						ImGui::SeparatorText("Transform: NA");

						ImGui::Separator;
					}


					ImGui::SeparatorText("Collider box:");

					//if (ImGui::InputFloat3("AABB scale", &aabb.scale[0]))
					//{
					//    // The slider was used; myVec3 has been updated.
					//    // You can handle the change here if needed.
					//}

					//ImGui::EndTabItem();

					ImGui::NewLine();

					// We were trying to add custom texture here via openLocalRepository() but it didn't work


					short int bitSetPos2 = COORD.GetComponentBitsetPos<c_Texture>();
					std::bitset<32> texBitset; // Create a bitset of size 32
					texBitset.set(bitSetPos2);

					std::bitset<32> entitySig2 = COORD.GetEntitySignature(entities[selected]);

					if ((entitySig2 & texBitset) == texBitset) // If this entity has a texture component
					{
						auto& texData = COORD.GetComponentDataFromEntity<c_Texture>(entities[selected]);
						ImGui::SeparatorText("Texture:");

						// Simple selection popup (if you want to show the current selection inside the Button itself,
						// you may want to build a string using the "###" operator to preserve a constant ID with a variable label)

						static int selected_fish = -1;
						const char* names[] = { "Wooden planks", "Rock" , "water", "grass", "lava" };
						static bool toggles[] = { true, false };

						if (ImGui::Button("Select texture"))
							ImGui::OpenPopup("my_select_popup");
						ImGui::SameLine();
						ImGui::TextUnformatted(selected_fish == -1 ? "<None>" : names[selected_fish]);
						if (ImGui::BeginPopup("my_select_popup"))
						{

							ImGui::SeparatorText("Default textures");
							for (int i = 0, j = 0; i < IM_ARRAYSIZE(names); i++)
							{
								if (ImGui::Selectable(names[i])) {
									selected_fish = i;
									switch (i) {
									case 0:
										texData.texUnit = containerTexUnit;
										modified.isModifed = true;
										break;

									case 1:
										texData.texUnit = rockySurfaceTexUnit;
										modified.isModifed = true;
										break;

									case 2:
										texData.texUnit = grassTexUnit;
										modified.isModifed = true;
										break;

									case 3:
										texData.texUnit = waterTexUnit;
										modified.isModifed = true;
										break;

									case 4:
										texData.texUnit = lavaTexUnit;
										modified.isModifed = true;
										break;

									default:
										break;
									}
								}

								std::cout << selectedOption << std::endl;

							}

							ImGui::EndPopup();
						}
					}
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Details"))
				{
					ImGui::Text("ID: !! Show EID here !!");
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();


			}
			ImGui::EndChild();

			ImGui::EndGroup();
		}
		
	}
	ImGui::End();
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