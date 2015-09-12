#include "AppFBX.h"

AppFBX::AppFBX() {}

int AppFBX::oninit()
{
	//shader source
	const char* vsSource = "#version 410\n\
							layout(location = 0) in vec4 Position;\
							layout(location = 1) in vec2 TexCoord;\
							out vec2 vTexCoord;\
							uniform mat4 ProjectionView;\
							void main()\
							{\
								vTexCoord = TexCoord;\
								gl_Position = ProjectionView * Position;\
							}";

	const char* fsSource = "#version 410\n\
							in vec2 vTexCoord;\
							out vec4 FragColor;\
							uniform sampler2D diffuse;\
							void main()\
							{\
								FragColor = texture(diffuse, vTexCoord);\
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
	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(programID, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}
	//cleanup
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);


	data = stbi_load("../fbx_models/soulspear/soulspear_diffuse.tga", &imageWidth, &imageHeight, &imageFormat, STBI_default);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(data);


	loadFBX("../fbx_models/soulspear/soulspear.fbx");
	//std::cout << getMeshCount();


	camera.setPerspective(glm::pi<float>() * 0.4f, 9.f / 16.f, .5f, 4000);
	camera.setSpeed(50, 0.1);

	Gizmos::create();

	return true;
}
void AppFBX::onkill()
{
	freeFBX();
	Gizmos::destroy();
}
bool AppFBX::onstep(float deltaTime)
{
	camera.update(deltaTime);

	return true;
}
void AppFBX::ondraw()
{
	//program
	glUseProgram(programID);
	int loc = glGetUniformLocation(programID, "ProjectionView");
	//texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	int loc2 = glGetUniformLocation(programID, "diffuse");
	glUniform1i(loc2, 0);
	//camera
	glUniformMatrix4fv(loc, 1, false, glm::value_ptr(camera.getProjectionView()));
	//draw
	for (unsigned int i = 0; i < glInfo.size(); i++)
	{
		glBindVertexArray(glInfo[i].VAO);
		glDrawElements(GL_TRIANGLES, glInfo[i].indexCount, GL_UNSIGNED_INT, 0);
	}

	//Gizmos ---------------------------
	Gizmos::clear();
	Gizmos::addTransform(glm::mat4(1));
	vec4 white(1);
	vec4 black(0, 0, 0, 1);

	for (int i = 0; i < 21; ++i)
	{
		Gizmos::addLine(vec3(-10 + i, 0, 10), vec3(-10 + i, 0, -10), i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i), vec3(-10, 0, -10 + i), i == 10 ? white : black);
	}
	Gizmos::draw(camera.getProjectionView());
	
}

void AppFBX::loadFBX(const char* path)
{
	file.load(path);
//	file.initialiseOpenGLTextures();

	glInfo.resize(file.getMeshCount());
	for (int i = 0; i < file.getMeshCount(); i++)
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

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)FBXVertex::Offsets::TexCoord1Offset);

		//index
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glInfo[i].IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m->m_indices.size(), m->m_indices.data(), GL_STATIC_DRAW);
		//unbind
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	}
	file.unload();
}
void AppFBX::freeFBX()
{
	file.unload();
}
int AppFBX::getMeshCount()
{
	return file.getMeshCount();
}
