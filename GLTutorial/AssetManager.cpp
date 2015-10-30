#include <gl_core_4_4.h>
#include "AssetLibrary.h"

using namespace AssetLibrary::ASSET;
const char* AssetLibrary::TYPE_NAMES[eSIZE + 1] = { "NONE","VAO","IBO","VBO","SIZE","FBO","RBO","Texture","Shader","SIZE" };

AssetLibrary::GL_Handle AssetLibrary::AssetManager::getVerified(const AssetKey &key) const
{
	if (handles.at(key))
	{
		return handles.at(key);
	}
	else
	{
		return NULL;
	}
}
bool AssetLibrary::AssetManager::setInternal(ASSET::GL_Handle_Type t, const char* name, GL_Handle handle)
{
	AssetKey key(t, name);
	if (handles.count(key))
	{
		std::cout << "Asset " << "<" << TYPE_NAMES[t] << ">" << name << " already exists." << std::endl;
		return false;
	}
	handles[key] = handle;
	std::cout << "Asset " << "<" << TYPE_NAMES[t] << ">" << name << " created!" << std::endl;
	return true;
}

bool AssetLibrary::AssetManager::buildVAO(const char* name, const struct Vertex *verts, unsigned int vsize, const unsigned int *indices, unsigned int isize)
{
	//check for copy
/*	if (get<ASSET::VAO>(name) != 0)
	{
		return false;
	}*/

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
	setInternal(ASSET::SIZE, name, isize);

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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * isize, indices, GL_STATIC_DRAW);
	//unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return true;
}

bool AssetLibrary::AssetManager::buildFBO(const char* name, unsigned int w, unsigned int h, unsigned int nTextures, const char* names[], const unsigned int depths[], bool hasDepth)
{
	/*if (get<ASSET::FBO>(name) != 0)
	{
		return false;
	}*/
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

bool AssetLibrary::AssetManager::buildTexture(const char* name, unsigned int w, unsigned int h, unsigned int depth, const unsigned char* pixels)
{
	/*if (get<ASSET::TEXTURE>(name) != 0)
	{
		return false;
	}*/
	GL_Handle texHandle;
	glGenTextures(1, &texHandle);
	setInternal(ASSET::TEXTURE, name, texHandle);

	glBindTexture(GL_TEXTURE_2D, texHandle);

	if (pixels != nullptr)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, depth, w, h, 0, depth, GL_UNSIGNED_BYTE, pixels);
	}
	else
	{
		glTexStorage2D(GL_TEXTURE_2D, 1, depth, w, h);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

bool AssetLibrary::AssetManager::loadTexture(const char* name, const char* path)
{
	int imageWidth, imageHeight, imageFormat = 0;
	unsigned int format;
	unsigned char* data;

	data = stbi_load(path, &imageWidth, &imageHeight, &imageFormat, STBI_default);

	switch (imageFormat)
	{
	case 1: format = GL_RED; break;
	case 2: format = GL_RG; break;
	case 3: format = GL_RGB; break;
	case 4: format = GL_RGBA; break;
	}

/*	GL_Handle texHandle;
	glGenTextures(1, &texHandle);
	setInternal(ASSET::TEXTURE, name, texHandle);
	*/
	buildTexture(name, imageWidth, imageHeight, format, data);
/*	glBindTexture(GL_TEXTURE_2D, texHandle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);*/

	stbi_image_free(data);

	return true;
}

bool AssetLibrary::AssetManager::loadShader(const char* name, const char* vertexpath, const char* fragmentpath)
{
	setInternal(ASSET::SHADER, name, ShaderLoader::createShaderProgram(vertexpath, fragmentpath));
	return true;
}

bool AssetLibrary::AssetManager::loadFBX(const char* name, const char* path)
{
	FBXFile file;
	file.load(path);
	//	file.initialiseOpenGLTextures();
	printf("Load FBX Start:\n");

	for (unsigned int i = 0; i < file.getMeshCount(); i++)
	{
		auto m = file.getMeshByIndex(i);
		//load vertecies
		std::vector<Vertex> fbxData(m->m_vertices.size());
		fbxData.data();
		for (unsigned j = 0; j < m->m_vertices.size(); j++)
		{
			fbxData[j].position = m->m_vertices[j].position;
			fbxData[j].normal = m->m_vertices[j].normal;
			fbxData[j].tangent = m->m_vertices[j].tangent;
			fbxData[j].texcoord = m->m_vertices[j].texCoord1;
		}
		buildVAO(m->m_name.c_str(), fbxData.data(), m->m_vertices.size(), m->m_indices.data(), m->m_indices.size());

		//Diffuse Map
		if (m->m_material->textures[FBXMaterial::DiffuseTexture] != NULL)
		{
			loadTexture(m->m_material->textures[FBXMaterial::DiffuseTexture]->name.c_str(), m->m_material->textures[FBXMaterial::DiffuseTexture]->path.c_str());
		}
		//Normal Map
		if (m->m_material->textures[FBXMaterial::NormalTexture] != NULL)
		{
			loadTexture(m->m_material->textures[FBXMaterial::NormalTexture]->name.c_str(), m->m_material->textures[FBXMaterial::NormalTexture]->path.c_str());
		}
		//Specular Map
		if (m->m_material->textures[FBXMaterial::SpecularTexture] != NULL)
		{
			loadTexture(m->m_material->textures[FBXMaterial::SpecularTexture]->name.c_str(), m->m_material->textures[FBXMaterial::SpecularTexture]->path.c_str());
		}
	}
	file.unload();
	printf("Load FBX End.\n");
	return true;
}

bool AssetLibrary::AssetManager::loadOBJ(const char* name, const char* path)
{
	return false;
}

bool AssetLibrary::AssetManager::init()
{
	setInternal(ASSET::FBO, "Screen", 0);

	buildVAO("Quad", QuadVerts, 4, QuadIndex, 6);
	buildVAO("Cube", CubeVerts, 24, CubeIndex, 36);

	unsigned char test[] = { 255, 255, 255, 255,   255, 255, 0, 255,
							 0, 255, 0, 255,   0, 0, 255, 255 };

	//unsigned char t2[] = {255,255,0,255};
	buildTexture("Test", 2, 2, GL_RGBA, test);
	unsigned char white[] = { 255, 255, 255, 255 };
	buildTexture("White", 1, 1, GL_RGBA, white);
	unsigned char flat[] = { 127, 127, 255, 255 };
	buildTexture("Flat", 1, 1, GL_RGBA, flat);

	loadTexture("LoadTest", "../resources/skyimagetest.png");

	//loadShader("ForwardShader", "./VertexShader.txt", "./FragmentShader.txt");

	return true;
}
void AssetLibrary::AssetManager::kill()
{
	for each(std::pair<AssetKey, GL_Handle> k in handles)
	{
		switch (k.first.first)
		{
		case ASSET::VBO:		glDeleteBuffers(1, &k.second);			break;
		case ASSET::IBO:		glDeleteBuffers(1, &k.second);			break;
		case ASSET::VAO:		glDeleteVertexArrays(1, &k.second);		break;
		case ASSET::SHADER:		glDeleteShader(k.second);				break;
		case ASSET::TEXTURE:	glDeleteTextures(1, &k.second);			break;
		case ASSET::RBO:		glDeleteRenderbuffers(1, &k.second);	break;
		case ASSET::FBO:		glDeleteFramebuffers(1, &k.second);		break;
		}
	}
}