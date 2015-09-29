#include "SkyBox.h"

SkyBox::SkyBox() {}
SkyBox::~SkyBox()
{

}

void SkyBox::create()
{
	float vertexData[] =
	{
		-1,-1,-1, 1,//0 left bottom front
		-1, 1,-1, 1,//1 left top front
		-1, 1, 1, 1,//2 left top back
		-1,-1, 1, 1,//3 left bottom back
		 1,-1,-1, 1,//4 right bottom front
		 1, 1,-1, 1,//5 right top front
		 1, 1, 1, 1,//6 right top back
		 1,-1, 1, 1,//7 right bottom back
	};
	unsigned int indexData[] =
	{   //right 4567
		4,5,6,
		4,7,6,
		//left 0123
		0,1,2,
		0,3,2,
		//top 1256
		1,2,6,
		1,5,6,
		//bottom 0347
		0,3,7,
		0,4,7,
		//back 2367
		2,3,7,
		2,6,7,
		//front 0145
		0,1,5,
		0,4,5,
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 8, vertexData, GL_STATIC_DRAW);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 36, indexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	glGenTextures(1, &cubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	programID = ShaderLoader::createShaderProgram("./SkyBoxShader/SkyBoxVertex.txt", "./SkyBoxShader/SkyBoxFragment.txt");
}

void SkyBox::loadCubeMap(const char* right, const char* left, const char* top, const char* bottom, const char* back, const char* front)
{
	int imageWidth, imageHeight, imageFormat = 0;
	unsigned char* data;
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

	data = stbi_load(right, &imageWidth, &imageHeight, &imageFormat, STBI_default);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	data = stbi_load(left, &imageWidth, &imageHeight, &imageFormat, STBI_default);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	data = stbi_load(top, &imageWidth, &imageHeight, &imageFormat, STBI_default);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	data = stbi_load(bottom, &imageWidth, &imageHeight, &imageFormat, STBI_default);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	data = stbi_load(back, &imageWidth, &imageHeight, &imageFormat, STBI_default);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	data = stbi_load(front, &imageWidth, &imageHeight, &imageFormat, STBI_default);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	stbi_image_free(data);
}

void SkyBox::draw(Camera camera)
{
	glUseProgram(programID);
	glBindVertexArray(VAO);
	/*DISABLE DEPTH*/glDisable(GL_DEPTH_TEST);

	int projection = glGetUniformLocation(programID, "Projection");
	int view = glGetUniformLocation(programID, "View");
	
	glUniformMatrix4fv(projection, 1, false, value_ptr(camera.getProjection()));
	mat4 inputView = camera.getView();
	inputView[3][0] = 0;
	inputView[3][1] = 0;
	inputView[3][2] = 0;
	glUniformMatrix4fv(view, 1, false, value_ptr(inputView));

	int textureLoc = glGetUniformLocation(programID, "cubeMap");
	glUniform1i(textureLoc, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
	

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glUseProgram(0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	/*ReEnable DEPTH*/glEnable(GL_DEPTH_TEST);
}