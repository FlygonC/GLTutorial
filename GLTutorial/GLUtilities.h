#pragma once

#include <gl_core_4_4.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>

#include "ShaderLoader.h"

struct OpenGLInfo
{
	unsigned int VAO;
	unsigned int VBO;
	unsigned int IBO;
	unsigned int indexCount;
};

struct SimpleFrameBuffer
{
	unsigned int FBO, texture, depth;

	~SimpleFrameBuffer()
	{
		glDeleteFramebuffers(1, &FBO);
		glDeleteTextures(1, &texture);
		glDeleteRenderbuffers(1, &depth);
	}

	void create(int screenWidth, int screenHeight)
	{
		glGenFramebuffers(1, &FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, screenWidth, screenHeight);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);

		glGenRenderbuffers(1, &depth);
		glBindRenderbuffer(GL_RENDERBUFFER, depth);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, screenWidth, screenHeight);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth);

		GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, drawBuffers);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
};

struct GPassFrameBuffer
{
	unsigned int FBO, albedoTexture, positionTexture, normalTexture, specularMap, depth;

	void create(int width, int height)
	{
		// setup gpass framebuffer
		glGenFramebuffers(1, &FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);

		glGenTextures(1, &albedoTexture);
		glBindTexture(GL_TEXTURE_2D, albedoTexture);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, width, height);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glGenTextures(1, &positionTexture);
		glBindTexture(GL_TEXTURE_2D, positionTexture);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB32F, width, height);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glGenTextures(1, &normalTexture);
		glBindTexture(GL_TEXTURE_2D, normalTexture);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB32F, width, height);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glGenTextures(1, &specularMap);
		glBindTexture(GL_TEXTURE_2D, specularMap);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA16F, width, height);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glGenRenderbuffers(1, &depth);
		glBindRenderbuffer(GL_RENDERBUFFER, depth);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, albedoTexture, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, positionTexture, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, normalTexture, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, specularMap, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth);

		GLenum gpassTargets[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		glDrawBuffers(4, gpassTargets);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
};

struct LightFrameBuffer
{
	unsigned int FBO, lightTexture, specularTexture;

	void create(int width, int height)
	{
		// setup light framebuffer
		glGenFramebuffers(1, &FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);

		glGenTextures(1, &lightTexture);
		glBindTexture(GL_TEXTURE_2D, lightTexture);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, width, height);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glGenTextures(1, &specularTexture);
		glBindTexture(GL_TEXTURE_2D, specularTexture);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, width, height);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, lightTexture, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, specularTexture, 0);

		GLenum lightTargets[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
		glDrawBuffers(2, lightTargets);
		auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			printf("FramebufferError!\n");
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
};

struct FullScreenQuad
{
	unsigned int VAO, VBO, IBO, programID, texture;

	FullScreenQuad() {}
	~FullScreenQuad()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &IBO);
		glDeleteProgram(programID);
	}

	void create(int screenWidth, int screenHeight)
	{
		glm::vec2 halftexel = 1.f / glm::vec2(screenWidth, screenHeight) * 0.5f;

		float vertexData[] = {
			-1,-1, 0, 1, (halftexel.x), (halftexel.y),//bot left  0
			1,-1, 0, 1, (1 - halftexel.x), (halftexel.y),//bot right 1
			1, 1, 0, 1, (1 - halftexel.x), (1 - halftexel.y),//top right 2
			-1, 1, 0, 1, (halftexel.x), (1 - halftexel.y),//top left  3
		};
		unsigned int indexData[] = {
			0, 3, 1, 2,
		};

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, vertexData, GL_STATIC_DRAW);

		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 4, indexData, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, ((char*)0) + 16);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		programID = ShaderLoader::createShaderProgram("./PostProcess/VertexSimple.txt", "./PostProcess/FragmentEffects.txt");
	}
	void draw()
	{
		glUseProgram(programID);
		glBindVertexArray(VAO);

		int textureLoc = glGetUniformLocation(programID, "target");
		glUniform1i(textureLoc, 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0);

		glUseProgram(0);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
};

struct FullScreenQuadS
{
	unsigned int VAO, VBO, IBO;

	FullScreenQuadS() {}
	~FullScreenQuadS()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &IBO);
	}

	void create(int screenWidth, int screenHeight)
	{
		glm::vec2 halftexel = 1.f / glm::vec2(screenWidth, screenHeight) * 0.5f;

		float vertexData[] = {
			-1,-1, 0, 1, (halftexel.x), (halftexel.y),//bot left  0
			1,-1, 0, 1, (1 - halftexel.x), (halftexel.y),//bot right 1
			1, 1, 0, 1, (1 - halftexel.x), (1 - halftexel.y),//top right 2
			-1, 1, 0, 1, (halftexel.x), (1 - halftexel.y),//top left  3
		};
		unsigned int indexData[] = {
			0, 3, 1, 2,
		};

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, vertexData, GL_STATIC_DRAW);

		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 4, indexData, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, ((char*)0) + 16);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	void draw()
	{
		glBindVertexArray(VAO);

		glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
	}
};

struct PartScreenQuad
{
	unsigned int VAO, VBO, IBO, programID, texture;

	PartScreenQuad() {}
	~PartScreenQuad()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &IBO);
		glDeleteProgram(programID);
	}

	void create(int screenWidth, int screenHeight, float p)
	{
		glm::vec2 halftexel = 1.f / glm::vec2(screenWidth, screenHeight) * 0.5f;
		float q = -1 + p * 2;

		float vertexData[] = {
			-1,-1, 0, 1, (halftexel.x), (halftexel.y),//bot left  0
			 q,-1, 0, 1, (1 - halftexel.x), (halftexel.y),//bot right 1
			 q, q, 0, 1, (1 - halftexel.x), (1 - halftexel.y),//top right 2
			-1, q, 0, 1, (halftexel.x), (1 - halftexel.y),//top left  3
		};
		unsigned int indexData[] = {
			0, 3, 1, 2,
		};

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, vertexData, GL_STATIC_DRAW);

		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 4, indexData, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, ((char*)0) + 16);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		programID = ShaderLoader::createShaderProgram("./PostProcess/VertexSimple.txt", "./PostProcess/FragmentEffects.txt");
	}
	void draw()
	{
		glUseProgram(programID);
		glBindVertexArray(VAO);

		int textureLoc = glGetUniformLocation(programID, "target");
		glUniform1i(textureLoc, 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0);

		glUseProgram(0);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
};

struct SimpleCube
{
	unsigned int VAO, VBO, IBO;

	void create()
	{
		float vertexData[] = {
			-1, -1, 1, 1,
			1, -1, 1, 1,
			1, -1, -1, 1,
			-1, -1, -1, 1,
			-1, 1, 1, 1,
			1, 1, 1, 1,
			1, 1, -1, 1,
			-1, 1, -1, 1,
		};
		unsigned int indexData[] = {
			0, 5, 4,
			0, 1, 5,
			1, 6, 5,
			1, 2, 6,
			2, 7, 6,
			2, 3, 7,
			3, 4, 7,
			3, 0, 4,
			4, 6, 7,
			4, 5, 6,
			3, 1, 0,
			3, 2, 1
		};

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8 * 4, vertexData, GL_STATIC_DRAW);

		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 36, indexData, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	void draw()
	{
		glBindVertexArray(VAO);

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

		glBindVertexArray(0);
	}
};