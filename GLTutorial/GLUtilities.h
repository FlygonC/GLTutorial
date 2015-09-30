#pragma once

#include <gl_core_4_4.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>

#include "ShaderLoader.h"

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
	unsigned int FBO, albedoTexture, positionTexture, normalTexture, depth;

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

		glGenRenderbuffers(1, &depth);
		glBindRenderbuffer(GL_RENDERBUFFER, depth);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, albedoTexture, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, positionTexture, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, normalTexture, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth);

		GLenum gpassTargets[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		glDrawBuffers(3, gpassTargets);
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