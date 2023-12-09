#include "Mesh.h"
#include "../vendor/stb_image/stb_image.h"

void Mesh::processMesh()
{

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

void Mesh::processTextures()
{

	//Get diffuse textures from the mesh:
	assimpMaterial = assimpScene->mMaterials[assimpMesh->mMaterialIndex];
	
	diffuseTextures = loadSpecularTextureFromMaterial();
	specularTextures = loadDiffuseTextureFromMaterial();

}

struct Texture
{
	unsigned int texHandle;
	std::string type; //diffuse, specular, etc.
	std::string fullPath;
};

std::vector<Texture> Mesh::loadSpecularTextureFromMaterial()
{
	std::vector<Texture> specularTextures;
	std::string directory = modelRef->getObjFilePath();

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
		if(!skip)
		{
			Texture texture;
			texture.fullPath = fullFilePath;
			texture.texHandle = createGLTextureBuffer(texture.fullPath.c_str());
			texture.type = "specular";

			specularTextures.push_back(texture);
			modelRef->addLoadedTexture(texture);
		}
	}
	return specularTextures;
}

std::vector<Texture> Mesh::loadDiffuseTextureFromMaterial()
{
	std::vector<Texture> diffuseTextures;
	std::string directory = modelRef->getObjFilePath();

	for (unsigned int i = 0; i < assimpMaterial->GetTextureCount(aiTextureType_DIFFUSE); i++)
	{
		aiString localTexFilePath;

		assimpMaterial->GetTexture(aiTextureType_DIFFUSE, i, &localTexFilePath); //Get the local file path

		std::string localFilePath = std::string(localTexFilePath.C_Str());
		std::string fullFilePath = directory + '/' + localFilePath;

		bool skip = false;

		const std::vector<Texture>& textures_loaded = modelRef->getLoadedTexRef();

		for (unsigned int j = 0; j < textures_loaded.size(); j++) //#Optimization_using_hash_map_cachin
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
	return diffuseTextures;
}

unsigned int Mesh::createGLTextureBuffer(const char* filePath)
{
	unsigned int texHandle;
	glGenTextures(1, &texHandle);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filePath, &width, &height, &nrComponents, 0);

	if(data) // if data != nullptr
	{
		GLenum format;
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
