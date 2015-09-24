#pragma once

#include "Application.h"
#include "FlyCamera.h"
#include "FBXLoader.h"
#include "ShaderLoader.h"

//#include <aie\Gizmos.h>

#include <stb_image.h>
#include <FBXFile.h>
//#include <tiny_obj_loader.h>
#include <vector>
#include <fstream>

struct SimpleQuad
{

	unsigned int VAO, VBO, IBO, programID, texture;

	SimpleQuad() {}
	~SimpleQuad()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &IBO);
		glDeleteProgram(programID);
	}

	void create()
	{
		float vertexData[] = {
			-5, 0,  -5, 1, 0, 0,
			5, 0,  -5, 1, 1, 0,
			5, 10, -5, 1, 1, 1,
			-5, 10, -5, 1, 0, 1,
		};
		unsigned int indexData[] = {
			0, 1, 2,
			0, 2, 3,
		};

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, vertexData, GL_STATIC_DRAW);

		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, indexData, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, ((char*)0) + 16);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		const char* vsSource = "#version 410\n\
		layout(location = 0) in vec4 Position;\
		layout(location = 1) in vec2 TexCoord;\
		out vec2 vTexCoord;\
		uniform mat4 ProjectionView;\
		void main(){\
			vTexCoord = TexCoord;\
			gl_Position = ProjectionView * Position;\
		}";
		const char* fsSource = "#version 410\n\
		in vec2 vTexCoord;\
		out vec4 FragColor;\
		uniform sampler2D diffuse;\
		void main(){\
			FragColor = texture(diffuse, vTexCoord);\
		}";
		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, (const char**)&vsSource, 0);
		glCompileShader(vertexShader);
		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, (const char**)&fsSource, 0);
		glCompileShader(fragmentShader);
		programID = glCreateProgram();
		glAttachShader(programID, vertexShader);
		glAttachShader(programID, fragmentShader);
		glLinkProgram(programID);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	void draw(glm::mat4 camera)
	{
		glUseProgram(programID);
		glBindVertexArray(VAO);

		int loc = glGetUniformLocation(programID, "ProjectionView");
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(camera));

		int textureLoc = glGetUniformLocation(programID, "diffuse");
		glUniform1i(textureLoc, 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glUseProgram(0);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
};

class AppDeferred : public Application
{
public:
	AppDeferred();

	unsigned int FBO, FBOTexture, FBODepth;
	unsigned int FBXShader;

	FBXLoader fbxData;
	FlyCamera camera;

	SimpleQuad quad;

	int oninit();
	void onkill();

	bool onstep(float deltaTime);
	void ondraw();

	void FBXDraw();
};