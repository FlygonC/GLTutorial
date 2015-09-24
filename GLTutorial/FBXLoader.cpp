#include "FBXLoader.h"

void FBXLoader::loadFBX(const char* path)
{
	file.load(path);
	//	file.initialiseOpenGLTextures();

	glInfo.resize(file.getMeshCount());
	for (unsigned int i = 0; i < file.getMeshCount(); i++)
	{
		auto m = file.getMeshByIndex(i);
		//m->m_indices.data();
		//m->m_vertices;
		glInfo[i].indexCount = m->m_indices.size();

		glGenVertexArrays(1, &glInfo[i].VAO);
		glGenBuffers(1, &glInfo[i].VBO);
		glGenBuffers(1, &glInfo[i].IBO);
		//array
		glBindVertexArray(glInfo[i].VAO);
		//vertex
		glBindBuffer(GL_ARRAY_BUFFER, glInfo[i].VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(FBXVertex) * m->m_vertices.size(), m->m_vertices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)FBXVertex::Offsets::PositionOffset);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)FBXVertex::Offsets::NormalOffset);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)FBXVertex::Offsets::TangentOffset);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)FBXVertex::Offsets::TexCoord1Offset);

		//index
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glInfo[i].IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m->m_indices.size(), m->m_indices.data(), GL_STATIC_DRAW);
		//unbind
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


		//Diffuse Map
		if (m->m_material->textures[FBXMaterial::DiffuseTexture] != NULL)
		{
			int imageWidth, imageHeight, imageFormat = 0;
			unsigned char* data;

			data = stbi_load(m->m_material->textures[FBXMaterial::DiffuseTexture]->path.c_str(), &imageWidth, &imageHeight, &imageFormat, STBI_default);

			glGenTextures(1, &glInfo[i].diffuse);
			glBindTexture(GL_TEXTURE_2D, glInfo[i].diffuse);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			//glBindTexture(GL_TEXTURE_2D, 0);

			stbi_image_free(data);
		}
		//Normal Map
		if (m->m_material->textures[FBXMaterial::NormalTexture] != NULL)
		{
			int imageWidth, imageHeight, imageFormat = 0;
			unsigned char* data;

			data = stbi_load(m->m_material->textures[FBXMaterial::NormalTexture]->path.c_str(), &imageWidth, &imageHeight, &imageFormat, STBI_default);

			glGenTextures(1, &glInfo[i].normal);
			glBindTexture(GL_TEXTURE_2D, glInfo[i].normal);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			//glBindTexture(GL_TEXTURE_2D, 0);

			stbi_image_free(data);
		}
		//Specular Map
		if (m->m_material->textures[FBXMaterial::SpecularTexture] != NULL)
		{
			int imageWidth, imageHeight, imageFormat = 0;
			unsigned char* data;

			data = stbi_load(m->m_material->textures[FBXMaterial::SpecularTexture]->path.c_str(), &imageWidth, &imageHeight, &imageFormat, STBI_default);

			glGenTextures(1, &glInfo[i].specular);
			glBindTexture(GL_TEXTURE_2D, glInfo[i].specular);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			//glBindTexture(GL_TEXTURE_2D, 0);

			stbi_image_free(data);
		}
	}
	//file.unload();
}

void FBXLoader::freeFBX()
{
	file.unload();
}

int FBXLoader::getMeshCount()
{
	return file.getMeshCount();
}

