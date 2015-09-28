#include "Sky.h"

Sky::Sky() {}
Sky::~Sky() {}

void Sky::create(float aspect_x, float aspect_y)
{
	float halfHeight = 0.125f;
	float halfWidth = halfHeight / (aspect_y / aspect_x);

	float vertexData[] = {
		-1,-1, 0, 1, halfWidth, halfHeight,//bot left  0
		 1,-1, 0, 1, -halfWidth, halfHeight,//bot right 1
		 1, 1, 0, 1, -halfWidth, -halfHeight,//top right 2
		-1, 1, 0, 1, halfWidth, -halfHeight,//top left  3
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

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);

	programID = ShaderLoader::createShaderProgram("./SkyShader/SkyVertex.txt", "./SkyShader/SkyFragment.txt");
}

void Sky::loadTexture(const char* path)
{
	int imageWidth, imageHeight, imageFormat = 0;
	unsigned char* data;

	data = stbi_load(path, &imageWidth, &imageHeight, &imageFormat, STBI_default);

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(data);
}

void Sky::draw(float view_x, float view_y)
{
	glUseProgram(programID);
	glBindVertexArray(VAO);
	/*DISABLE DEPTH*/glDisable(GL_DEPTH_TEST);

	int textureLoc = glGetUniformLocation(programID, "image");
	glUniform1i(textureLoc, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	int loc = glGetUniformLocation(programID, "ViewX");
	glUniform1f(loc, view_x);
	loc = glGetUniformLocation(programID, "ViewY");
	glUniform1f(loc, view_y);

	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0);

	glUseProgram(0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	/*ReEnable DEPTH*/glEnable(GL_DEPTH_TEST);
}