#include "Mesh.h"
#include "Model.h"

Mesh::Mesh(aiMesh* mesh, const aiScene* scene, Model* model) : modelRef(model), assimpScene(scene) //scene is needed when you want to retrieve the textures from the material.
{
	//Store input data:
	assimpMesh = mesh;

	//Collect vertex data:
	processVertices();

	//Collect Indices:
	processIndices();

	//Collect Textures from materials;
	processTextures();

	//After calling these 3 functions, we should have stored all the data into the relevant data structures in mesh.
	//Texture is already assigned to a texture object referenced by texHandle in the Texture object defined in Model.h
	//Now, assign that data to VAO, VBO and EBO so it can be stored in the VRAM and accessed by openGL
	processDataForOpenGl();
}

//***NOTE***: Potential Error here: If my shader has lets say 3 slots for specular textures, then mesh 1 assigns 3 but mesh 2 only assigns one
//		because they all use they same shader, mesh 2 will still use the specular textures assigned from mesh 1, which could result in some strange effects.
//		It's worth looking into a solution for this later:
//		At present this only works if all meshes have an equal number of textures (both diffuse and specular) or each subsequent mesh has more or = textures from the previosu mesh.
void Mesh::DrawMesh(Shader* shader)
{
	shader->bindProgram();

	unsigned int No_diffuse = 1;
	unsigned int No_specular = 1;

	
	//Shader* theShader = &shader;

	//Assign the texture units for this mesh:
	unsigned int i = 0;
	for(i; i < diffuseTextures.size(); i++)
	{
		GLCALL(glActiveTexture(GL_TEXTURE0 + i));

		std::string number = std::to_string(No_diffuse++);
		
		//GLCALL(glUniform1i(glGetUniformLocation(shader.getShaderHandle(), ("texture_diffuse" + number).c_str()), texUnit));
		std::string name = "texture_diffuse" + number;
		shader->setUniformTextureUnit(name, i);
		glBindTexture(GL_TEXTURE_2D, diffuseTextures[i].texHandle);
		//texUnit++;
	}

	int size = i;

	for (i; i < specularTextures.size() + size; i++)
	{
		GLCALL(glActiveTexture(GL_TEXTURE0 + i));

		std::string number = std::to_string(No_specular++);

		//GLCALL(glUniform1i(glGetUniformLocation(shader.getShaderHandle(), ("texture_specular" + number).c_str()), texUnit));
		std::string name = "texture_specular" + number;
		shader->setUniformTextureUnit(name, i);
		glBindTexture(GL_TEXTURE_2D, specularTextures[i - size].texHandle);
		//texUnit++;
	}

	GLCALL(glBindVertexArray(VAO));
	GLCALL(glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0));

	// Unbind everything for safety
	GLCALL(glBindVertexArray(0));
	GLCALL(glActiveTexture(GL_TEXTURE0));
}

void Mesh::processVertices()
{
	for (unsigned int i = 0; i < assimpMesh->mNumVertices; i++)
	{
		Vertex vert;

		processVertPositionData(&vert, i);
		processVertNormalData(&vert, i);
		processVertTexCoords(&vert, i);

		vertices.push_back(vert);
	}
}

void Mesh::processIndices()
{
	for (unsigned int i = 0; i < assimpMesh->mNumFaces; i++)
	{
		aiFace face = assimpMesh->mFaces[i];
		
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}
}

void Mesh::loadSpecularTextureFromMaterial()
{
	//std::vector<Texture> specularTextures;
	std::string directory = modelRef->getObjFilePath();
	directory = directory.substr(0, directory.find_last_of('/'));

	for (unsigned int i = 0; i < assimpMaterial->GetTextureCount(aiTextureType_SPECULAR); i++)
	{
		aiString localTexFilePath;

		assimpMaterial->GetTexture(aiTextureType_SPECULAR, i, &localTexFilePath); //Get the local file path

		std::string localFilePath = std::string(localTexFilePath.C_Str());
		std::string fullFilePath = directory + '/' + localFilePath;

		bool skip = false;

		const std::vector<Texture>& textures_loaded = modelRef->getLoadedTexRef();

		for (unsigned int j = 0; j < textures_loaded.size(); j++) //#Optimization_using_hash_map_cachin
		{
			if (std::strcmp(textures_loaded[j].fullPath.data(), fullFilePath.c_str()) == 0)
			{
				specularTextures.push_back(textures_loaded[j]);
				skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip)
		{
			Texture texture;
			texture.fullPath = fullFilePath;
			texture.texHandle = createGLTextureBuffer(texture.fullPath.c_str());
			texture.type = "specular";

			specularTextures.push_back(texture);
			modelRef->addLoadedTexture(texture);
		}
	}
	//return specularTextures;
}

void Mesh::loadDiffuseTextureFromMaterial()
{
	//std::vector<Texture> diffuseTextures;
	std::string directory = modelRef->getObjFilePath();
	directory = directory.substr(0, directory.find_last_of('/'));

	for (unsigned int i = 0; i < assimpMaterial->GetTextureCount(aiTextureType_DIFFUSE); i++)
	{
		aiString localTexFilePath;

		assimpMaterial->GetTexture(aiTextureType_DIFFUSE, i, &localTexFilePath); //Get the local file path

		std::string localFilePath = std::string(localTexFilePath.C_Str());
		std::string fullFilePath = directory + '/' + localFilePath;

		bool skip = false;

		const std::vector<Texture>& textures_loaded = modelRef->getLoadedTexRef();

		for (unsigned int j = 0; j < textures_loaded.size(); j++) //#Optimization_using_hash_map_caching
		{
			if (std::strcmp(textures_loaded[j].fullPath.data(), fullFilePath.c_str()) == 0)
			{
				diffuseTextures.push_back(textures_loaded[j]);
				skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip)
		{
			Texture texture;
			texture.fullPath = fullFilePath;
			texture.texHandle = createGLTextureBuffer(texture.fullPath.c_str());
			texture.type = "diffuse";

			diffuseTextures.push_back(texture);
			modelRef->addLoadedTexture(texture);
		}
	}
	//return diffuseTextures;
}


void Mesh::processTextures()
{

	//Get diffuse textures from the mesh:
	assimpMaterial = assimpScene->mMaterials[assimpMesh->mMaterialIndex];
	
	loadDiffuseTextureFromMaterial();
	loadSpecularTextureFromMaterial();

}

unsigned int Mesh::createGLTextureBuffer(const char* filePath)
{
	unsigned int texHandle;
	glGenTextures(1, &texHandle);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filePath, &width, &height, &nrComponents, 0);

	if(data) // if data != nullptr
	{
		GLenum format = GL_RGBA;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, texHandle);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << filePath << std::endl;
		ASSERT(false);
		stbi_image_free(data);
	}
	return texHandle;
}

void Mesh::processDataForOpenGl()
{
	GLCALL(glGenVertexArrays(1, &VAO));
	GLCALL(glGenBuffers(1, &VBO));
	GLCALL(glGenBuffers(1, &EBO));

	GLCALL(glBindVertexArray(VAO));

	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	GLCALL(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW));

	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
	GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW));

	GLCALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0));
	GLCALL(glEnableVertexAttribArray(0));
	GLCALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, norm)));
	GLCALL(glEnableVertexAttribArray(1));
	GLCALL(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCord)));
	GLCALL(glEnableVertexAttribArray(2));

	GLCALL(glBindVertexArray(0)); //Unbind the current VAO (for safety purposes)
}

void Mesh::processVertPositionData(Vertex* vert, unsigned int i)
{
	if (assimpMesh->HasPositions())
	{
		glm::vec3 vec;
		vec.x = assimpMesh->mVertices[i].x;
		vec.y = assimpMesh->mVertices[i].y;
		vec.z = assimpMesh->mVertices[i].z;

		vert->pos = vec;
	}
}

void Mesh::processVertNormalData(Vertex* vert, unsigned int i)
{
	if (assimpMesh->HasNormals())
	{
		glm::vec3 vec;
		vec.x = assimpMesh->mNormals[i].x;
		vec.y = assimpMesh->mNormals[i].y;
		vec.z = assimpMesh->mNormals[i].z;

		vert->norm = vec;
	}
}

void Mesh::processVertTexCoords(Vertex* vert, unsigned int i)
{
	if (assimpMesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
	{
		glm::vec2 vec;
		vec.x = assimpMesh->mTextureCoords[0][i].x;
		vec.y = assimpMesh->mTextureCoords[0][i].y;

		vert->texCord = vec;
	}
}
