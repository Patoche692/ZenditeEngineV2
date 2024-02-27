#pragma once
#include "I_API_Manager.h"
#include "../ECS/Components.h"
#include "../TextureData.h"

class OpenGL_Manager : public I_API_Manager
{
private:
	std::vector<std::shared_ptr<TextureData>> textures;

public:
	OpenGL_Manager()
	{
		No_texUnits = 0;
	}

	void SetupRenderData(Entity EID, std::shared_ptr<ECSCoordinator> ECScoord) override
	{
		R_DataHandle DH;
		c_RenderableComponent posVertexData = ECScoord->GetComponentDataFromEntity<c_RenderableComponent>(EID);
		float* posVertices = posVertexData.posVertices;
		float* surfaceNormalVertices = posVertexData.surfaceNormalVertices;
		//#HERE CREATE a surface normal vertex array.
		size_t posArraySize = posVertexData.posArraySize;
		size_t snArraySize = posVertexData.snArraySize;

		//#NOTE Use the DH.bitset value to determine what data to setup for the vertex data passed in
		// (For this version do a simple set up for testing purposes.

		GLCALL(glGenVertexArrays(1, &(DH.VAO)));
		GLCALL(glBindVertexArray(DH.VAO));

		GLCALL(glGenBuffers(1, &(DH.posVBO)));
		GLCALL(glGenBuffers(1, &(DH.surfaceNormalVBO)));

		//Position data:
		GLCALL(glBindBuffer(GL_ARRAY_BUFFER, DH.posVBO));
		GLCALL(glBufferData(GL_ARRAY_BUFFER, posArraySize, posVertices, GL_STATIC_DRAW));
		GLCALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));
		GLCALL(glEnableVertexAttribArray(0));

		//Surface Normal Data:
		GLCALL(glBindBuffer(GL_ARRAY_BUFFER, DH.surfaceNormalVBO));
		GLCALL(glBufferData(GL_ARRAY_BUFFER, snArraySize, surfaceNormalVertices, GL_STATIC_DRAW));
		GLCALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));
		GLCALL(glEnableVertexAttribArray(1));

		short int bitSetPos = ECScoord->GetComponentBitsetPos<c_Texture>();
		std::bitset<32> textureBitset; // Create a bitset of size 32
		textureBitset.set(bitSetPos);

		std::bitset<32> entitySig = ECScoord->GetEntitySignature(EID);

		if ((entitySig & textureBitset) == textureBitset) // If this entity has a texture component
		{
			c_Texture texVertexData = ECScoord->GetComponentDataFromEntity<c_Texture>(EID);
			size_t textureArraySize = texVertexData.arraySize;

			GLCALL(glGenBuffers(1, &(DH.texCoordVBO)));
			GLCALL(glBindBuffer(GL_ARRAY_BUFFER, DH.texCoordVBO));

			GLCALL(glBufferData(GL_ARRAY_BUFFER, textureArraySize, texVertexData.texCoords, GL_STATIC_DRAW));
			GLCALL(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0));
			GLCALL(glEnableVertexAttribArray(2));

			DH.texUnit = texVertexData.texUnit; //#TexUNIT_Set
			DH.texture = textures[texVertexData.texUnit];
		}
		else
		{
			
		}

		m_Map_ENTITYtoHANDLE[EID] = DH; //Insert modified DH into the map.
	
		GLCALL(glBindVertexArray(0)); //Unbind the VAO
	}

	void SetupAABBRenderData(Entity EID, std::shared_ptr<ECSCoordinator> ECScoord)
	{
		R_DataHandle DH;

		GLCALL(glGenVertexArrays(1, &(DH.AABB_VAO)));
		GLCALL(glBindVertexArray(DH.AABB_VAO));

		c_AABB vertexDataAABB = ECScoord->GetComponentDataFromEntity<c_AABB>(EID);
		GLCALL(glGenBuffers(1, &(DH.AABB_posVBO)));
		GLCALL(glBindBuffer(GL_ARRAY_BUFFER, DH.AABB_posVBO));
		GLCALL(glBufferData(GL_ARRAY_BUFFER, 36*3*sizeof(float), vertexDataAABB.vertices, GL_STATIC_DRAW));
		GLCALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));
		GLCALL(glEnableVertexAttribArray(0));

		if(m_Map_ENTITYtoHANDLE.find(EID) == m_Map_ENTITYtoHANDLE.end()) //If EID is not in the map
		{
			m_Map_ENTITYtoHANDLE[EID] = DH;
		}
		else
		{
			m_Map_ENTITYtoHANDLE[EID].AABB_VAO = DH.AABB_VAO;
			m_Map_ENTITYtoHANDLE[EID].AABB_posVBO = DH.AABB_posVBO;
		}
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

	std::shared_ptr<Shader> GetEntityShader(Entity EID) const
	{
		DEBUG_ASSERT(m_Map_ENTITYtoSHADER.find(EID) != m_Map_ENTITYtoSHADER.end(), "EID does not exist");

		auto it = m_Map_ENTITYtoSHADER.find(EID);

		return it->second;
	}

	void SetShaderForEntity(Entity EID, std::shared_ptr<Shader> shader) override
	{
		DEBUG_ASSERT(m_Map_ENTITYtoHANDLE.find(EID) != m_Map_ENTITYtoHANDLE.end(), "Entity does not exist");
		
		m_Map_ENTITYtoSHADER[EID] = shader;
	}

	void SetShaderForDataHandle(Entity EID) override
	{
		R_DataHandle& DH = GetNonConstEntityDataHandle(EID);

		DH.shader = m_Map_ENTITYtoSHADER[EID];
	}

	unsigned short int GenerateTexUnit(std::string texFilePath, std::string fileType) override
	{
		DEBUG_ASSERT(No_texUnits <= 16, "Cap on No. Texture Units has been exceeded"); //#IMPROVE Create a system to swap texture units that are not in use in and out
		
		//#SetUp Tex Unit here
		if(m_Map_FILEPATHtoTEXUNIT.find(texFilePath) == m_Map_FILEPATHtoTEXUNIT.end())
		{
			textures.push_back(std::make_shared<TextureData>(texFilePath));

			if(fileType == "JPG")
			{
				(textures[No_texUnits])->setupTextureJPG(No_texUnits, texFilePath);
			}
			else // PNG will be default 
			{
				(textures[No_texUnits])->setupTexturePNG(No_texUnits, texFilePath);
			}

			m_Map_FILEPATHtoTEXUNIT[texFilePath] = No_texUnits;

			++No_texUnits;
		}


		return m_Map_FILEPATHtoTEXUNIT[texFilePath];

	}

	void GenerateDepthMap(unsigned int& depthMapFBO, unsigned int& depthMapUnit) override
	{
		float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };

		glGenFramebuffers(1, &depthMapFBO);
		glGenTextures(1, &depthMapUnit);

		glBindTexture(GL_TEXTURE_2D, depthMapUnit);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapUnit, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
};