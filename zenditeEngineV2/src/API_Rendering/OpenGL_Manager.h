#pragma once
#include "I_API_Manager.h"
#include "../ECS/Components.h"
#include "../Texture2D.h"

class OpenGL_Manager : public I_API_Manager
{
public:
	void SetupRenderData(Entity EID, std::shared_ptr<ECSCoordinator> ECScoord) override
	{
		R_DataHandle DH;
		c_RenderableComponent vertexData = ECScoord->GetComponentDataFromEntity<c_RenderableComponent>(EID);
		float* vertices = vertexData.vertices;

		//#NOTE Use the DH.bitset value to determine what data to setup for the vertex data passed in
		// (For this version do a simple set up for testing purposes.

		GLCALL(glGenVertexArrays(1, &(DH.VAO)));
		GLCALL(glBindVertexArray(DH.VAO));

		GLCALL(glGenBuffers(1, &(DH.VBO)));
		GLCALL(glBindBuffer(GL_ARRAY_BUFFER, DH.VBO));
		GLCALL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

		GLCALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0));
		GLCALL(glEnableVertexAttribArray(0));

		GLCALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(3 * sizeof(float))));
		GLCALL(glEnableVertexAttribArray(1));

		GLCALL(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(6 * sizeof(float))));
		GLCALL(glEnableVertexAttribArray(2));

		DH.texture = std::make_unique<Texture2D>("diffuse");
		(DH.texture)->setupTexturePNG(0, "res/textures/container2.png");


		ECScoord->GetComponentDataFromEntity<c_Modified>(EID).isModifed = false;

		m_Map_ENTITYtoHANDLE[EID] = DH; //Insert modified DH into the map.
	
		GLCALL(glBindVertexArray(0)); //Unbind the VAO
	}

	R_DataHandle const& GetEntityDataHandle(Entity EID) const override
	{
		auto it = m_Map_ENTITYtoHANDLE.find(EID);
		if (it != m_Map_ENTITYtoHANDLE.end()) {
			return it->second;
		}
		else {
			// Handle the case where the EID is not found. For example, throw an exception
			// or return a reference to a static default-constructed R_DataHandle object.
			static R_DataHandle defaultHandle;
			return defaultHandle;
		}
	}

	R_DataHandle& GetNonConstEntityDataHandle(Entity EID) override
	{
		auto it = m_Map_ENTITYtoHANDLE.find(EID);
		if (it != m_Map_ENTITYtoHANDLE.end()) {
			return it->second;
		}
		else {
			// Handle the case where the EID is not found. For example, throw an exception
			// or return a reference to a static default-constructed R_DataHandle object.
			DEBUG_ASSERT(false, "EID data handle Not found"); // Handle map is not retrieving the data handle (find out why)
			static R_DataHandle defaultHandle;
			return defaultHandle;
		}
	}

	void SetShaderForEntity(Entity EID, std::shared_ptr<Shader> shader) override
	{
		R_DataHandle& DH = GetNonConstEntityDataHandle(EID);

		DH.shader = shader;
	}

};