#include "AppFBX.h"

AppFBX::AppFBX() {}

int AppFBX::oninit()
{
	
	programID = createShaderProgram("./VertexShader.txt", "./FragmentShader.txt");

	texDiffuse = loadTexture("../fbx_models/soulspear/soulspear_diffuse.tga");
	texSpecular = loadTexture("../fbx_models/soulspear/soulspear_specular.tga");

	loadFBX("../fbx_models/soulspear/soulspear.fbx");
	//std::cout << getMeshCount();


	camera.setPerspective(glm::pi<float>() * 0.5f, 9.f / 16.f, .5f, 4000);
	camera.setSpeed(25, 0.1f);

	Gizmos::create();

	return true;
}
void AppFBX::onkill()
{
	glDeleteProgram(programID);
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
	int projection = glGetUniformLocation(programID, "ProjectionView");
	int ambientLight = glGetUniformLocation(programID, "AmbientLight");
	int lightDirection = glGetUniformLocation(programID, "LightDirection");
	int lightColor = glGetUniformLocation(programID, "LightColor");
	int cameraPosition = glGetUniformLocation(programID, "CameraPosition");
	int specularPower = glGetUniformLocation(programID, "SpecularPower");

	glUniformMatrix4fv(projection, 1, false, glm::value_ptr(camera.getProjectionView()));
	glUniform3fv(ambientLight, 1, glm::value_ptr(glm::vec3(0.f)));
	glUniform3fv(lightDirection, 1, glm::value_ptr( glm::normalize( glm::vec3(sin(getCurrentTime()), 1, 1) ) * 1.f ));
	glUniform3fv(lightColor, 1, glm::value_ptr(glm::vec3(1, 1, 1)));
	glUniform3fv(cameraPosition, 1, glm::value_ptr(camera.getWorldTransform()[3]));
	glUniform1f(specularPower, 128.f);

	//textures
	int textureLoc = glGetUniformLocation(programID, "Diffuse");
	glUniform1i(textureLoc, 0);

	textureLoc = glGetUniformLocation(programID, "Specular");
	glUniform1i(textureLoc, 1);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texDiffuse);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texSpecular);
	
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

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), 0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)FBXVertex::Offsets::NormalOffset);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)FBXVertex::Offsets::TexCoord1Offset);

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


unsigned int AppFBX::loadShader(unsigned int type, const std::string path)
{
	std::string source;
	std::ifstream inFile;
	inFile.open(path);
	if (!inFile)
	{
		std::cout << "Error: Could not read file!" << std::endl;
		return -1;
	}
	std::string line;
	while (inFile)
	{
		std::getline(inFile, line);
		if (inFile)
		{
			source += line + '\n';
		}
		else
		{
			source += '\0';
		}
	}
	
	const char* finalSource = source.c_str();
	//std::cout << finalSource;
	unsigned int shader = glCreateShader(type);
	glShaderSource(shader, 1, (const char**)&finalSource, 0);
	glCompileShader(shader);

	return shader;
}

unsigned int AppFBX::createShaderProgram(const std::string vertexShaderPath, const std::string fragmentShaderPath)
{
	//make shader variables
	int success = GL_FALSE;

	unsigned int vertexShader = loadShader(GL_VERTEX_SHADER, vertexShaderPath);
	unsigned int fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentShaderPath);

	//link
	unsigned int program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	//make sure it worked
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(program, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}
	//cleanup
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	return program;
}
void AppFBX::freeShaderProgram()
{

}

unsigned int AppFBX::loadTexture(const std::string path)
{
	unsigned int texture = -1;

	data = stbi_load(path.c_str(), &imageWidth, &imageHeight, &imageFormat, STBI_default);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(data);

	return texture;
}