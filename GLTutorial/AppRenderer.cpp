#include "AppRenderer.h"

AppRenderer::AppRenderer() {}

int AppRenderer::oninit()
{
	//shader source
	const char* vsSource =	"#version 410\n\
							layout(location= 0) in vec4 Position;\
							layout(location = 1) in vec4 Colour;\
							out vec4 vColour;\
							uniform mat4 ProjectionView;\
							void main()\
							{\
								vColour = Colour;\
								gl_Position = ProjectionView * Position;\
							}";

	const char* fsSource =	"#version 410\n\
							in vec4 vColour;\
							out vec4 FragColor;\
							void main()\
							{\
								FragColor = vColour;\
							}";
	//make shader variables
	int success = GL_FALSE;
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//compile
	glShaderSource(vertexShader, 1, (const char**)&vsSource, 0);
	glCompileShader(vertexShader);
	glShaderSource(fragmentShader, 1, (const char**)&fsSource, 0);
	glCompileShader(fragmentShader);
	//link
	programID = glCreateProgram();
	glAttachShader(programID, vertexShader);
	glAttachShader(programID, fragmentShader);
	glLinkProgram(programID);
	//make sure it worked
	glGetProgramiv(programID,GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetProgramiv(programID,GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(programID, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}
	//cleanup
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	//generateGrid(20, 20);

	err = tinyobj::LoadObj(shapes, materials, "../stanford_objs/dragon.obj");
	createOpenGLBuffers(shapes);


	camera.setPerspective(glm::pi<float>() * 0.4f, 9.f / 16.f, .5f, 4000);
	camera.setSpeed(100, 0.1);

	return true;
}

void AppRenderer::onkill()
{

}

bool AppRenderer::onstep(float deltaTime)
{
	camera.update(deltaTime);

	return true;
}

void AppRenderer::ondraw()
{ 
	glUseProgram(programID);
	unsigned int projectionViewUniform = glGetUniformLocation(programID, "ProjectionView");
	glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(camera.getProjectionView()));
	for (unsigned int i = 0; i < glInfo.size(); i++)
	{
		glBindVertexArray(glInfo[i].VAO);
		glDrawElements(GL_TRIANGLES, glInfo[i].indexCount, GL_UNSIGNED_INT, 0);
	}

}

/*
void AppRenderer::generateGrid(unsigned int prows, unsigned int columns)
{
	rows = prows;
	cols = columns;
	Vertex* aoVertices = new Vertex[rows * columns];
	for (unsigned int r = 0; r < rows; ++r)
	{
		for (unsigned int c = 0; c < columns; ++c)
		{
			aoVertices[r * columns + c].position = vec4((float)c, 0, (float)r, 1);
			// create some arbitrary colour based off something
			// that might not be related to tiling a texture
			vec3 colour = vec3(sinf((c / (float)(columns - 1)) *(r / (float)(rows - 1))));
			aoVertices[r * columns + c].color = vec4(colour, 1);
		}
	}

	//create index array
	unsigned int* auiIndices = new unsigned int[(rows - 1) * (columns - 1) * 6];

	unsigned int index = 0;
	for (unsigned int r = 0; r < (rows - 1); r++)
	{
		for (unsigned int c = 0; c < (columns - 1); c++)
		{
			// triangle 1
			auiIndices[index++] = r * columns + c;
			auiIndices[index++] = (r + 1) * columns + c;
			auiIndices[index++] = (r + 1) * columns + (c + 1);
			// triangle 2
			auiIndices[index++] = r * columns + c;
			auiIndices[index++] = (r + 1) * columns + (c + 1);
			auiIndices[index++] = r * columns + (c + 1);
		}
	}

	//create and bind buffers
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, (rows * columns) * sizeof(Vertex), aoVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec4)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (rows - 1) * (columns - 1) * 6 * sizeof(unsigned int), auiIndices, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	

	


	delete[] auiIndices;
	delete[] aoVertices;
}
*/

void AppRenderer::createOpenGLBuffers(std::vector<tinyobj::shape_t>& shapes)
{
	glInfo.resize(shapes.size());

	for (unsigned int meshIndex = 0; meshIndex < shapes.size(); meshIndex++)
	{
		glGenVertexArrays(1, &glInfo[meshIndex].VAO);
		glGenBuffers(1, &glInfo[meshIndex].VBO);
		glGenBuffers(1, &glInfo[meshIndex].IBO);
		glBindVertexArray(glInfo[meshIndex].VAO);

		unsigned int floatCount = shapes[meshIndex].mesh.positions.size();
		floatCount += shapes[meshIndex].mesh.normals.size();
		floatCount += shapes[meshIndex].mesh.texcoords.size();

		std::vector<float> vertexData;
		vertexData.reserve(floatCount);

		vertexData.insert(vertexData.end(), shapes[meshIndex].mesh.positions.begin(), shapes[meshIndex].mesh.positions.end());

		vertexData.insert(vertexData.end(), shapes[meshIndex].mesh.normals.begin(), shapes[meshIndex].mesh.normals.end());

		glInfo[meshIndex].indexCount = shapes[meshIndex].mesh.indices.size();

		glBindBuffer(GL_ARRAY_BUFFER, glInfo[meshIndex].VBO);
		glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glInfo[meshIndex].IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, shapes[meshIndex].mesh.indices.size() * sizeof(unsigned int), shapes[meshIndex].mesh.indices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * shapes[meshIndex].mesh.positions.size()));

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}