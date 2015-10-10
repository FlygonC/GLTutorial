#include "AssetLibrary.h"

AssetLibrary::GL_Handle AssetLibrary::AssetManager::getVerified(const AssetKey &key) const
{
	//Possible debug stuff here
	return handles.at(key);
}
bool AssetLibrary::AssetManager::setInternal(ASSET::GL_Handle_Type t, const char* name, GL_Handle handle)
{
	AssetKey key(t, name);
	if (handles.count(key))
	{
		return false;
	}
	handles[key] = handle;
	return true;
}

bool AssetLibrary::AssetManager::buildVAO(const char* name, const struct Vertex *verts, unsigned int vsize, const unsigned int *indices, unsigned int isize)
{
	//check for copy
	if (get<ASSET::VAO>(name) != 0)
	{
		return false;
	}

	GL_Handle vaoHandle;
	GL_Handle vboHandle;
	GL_Handle iboHandle;
	//Gen Buffers
	glGenVertexArrays(1, &vaoHandle);
	glGenBuffers(1, &vboHandle);
	glGenBuffers(1, &iboHandle);

	//Store handles in asset manager
	setInternal(ASSET::VAO, name, vaoHandle);
	setInternal(ASSET::VBO, name, vboHandle);
	setInternal(ASSET::IBO, name, iboHandle);

	//Set Data
	glBindVertexArray(vaoHandle);
	//vertex
	glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vsize, verts, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)(sizeof(glm::vec4)));
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 2));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 3));
	//index
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboHandle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * vsize, indices, GL_STATIC_DRAW);
	//unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return true;
}

bool AssetLibrary::AssetManager::buildFBO(const char* name, unsigned int w, unsigned int h, unsigned int nTextures, const char* names[], const unsigned int depths[], bool hasDepth)
{
	if (get<ASSET::FBO>(name) != 0)
	{
		return false;
	}
	GL_Handle fboHandle;
	glGenFramebuffers(1, &fboHandle);
	setInternal(ASSET::FBO, name, fboHandle);

	glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);
	unsigned *renderTargets = new unsigned[nTextures];
	int nColorAttachments = 0;
	for (unsigned i = 0; i < nTextures; i++)
	{
		buildTexture(names[i], w, h, depths[i]);
		glFramebufferTexture(GL_FRAMEBUFFER, renderTargets[i] = GL_COLOR_ATTACHMENT0 + nColorAttachments++, get<ASSET::TEXTURE>(names[i]), 0);
	}
	if (hasDepth)
	{
		unsigned int rboHandle;
		glGenRenderbuffers(1, &rboHandle);
		setInternal(ASSET::RBO, name, rboHandle);

		glBindRenderbuffer(GL_RENDERBUFFER, rboHandle);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboHandle);
	}

	glDrawBuffers(nColorAttachments, renderTargets);
	delete[] renderTargets;
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

bool AssetLibrary::AssetManager::buildTexture(const char* name, unsigned int w, unsigned int h, unsigned int depth, const char* pixles = nullptr)
{
	if (get<ASSET::TEXTURE>(name) != 0)
	{
		return false;
	}
	GL_Handle texHandle;
	glGenTextures(1, &texHandle);
	setInternal(ASSET::TEXTURE, name, texHandle);

	glBindTexture(GL_TEXTURE_2D, texHandle);

	glTexStorage2D(GL_TEXTURE_2D, 1, depth, w, h);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

bool AssetLibrary::AssetManager::loadTexture(const char* name, const char* path)
{
	int imageWidth, imageHeight, imageFormat = 0;
	unsigned char* data;

	data = stbi_load(path, &imageWidth, &imageHeight, &imageFormat, STBI_default);

	GL_Handle texHandle;
	glGenTextures(1, &texHandle);
	setInternal(ASSET::TEXTURE, name, texHandle);

	glBindTexture(GL_TEXTURE_2D, texHandle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(data);
}

bool AssetLibrary::AssetManager::loadShader(const char* name, const char* vertexpath, const char* fragmentpath)
{
	setInternal(ASSET::SHADER, name, ShaderLoader::createShaderProgram(vertexpath, fragmentpath));
}