#include "AppFBX.h"

AppFBX::AppFBX() {}

int AppFBX::oninit()
{
	
	programID = ShaderLoader::createShaderProgram("./VertexShader.txt", "./FragmentShader.txt");

	//texDiffuse = loadTexture("../fbx_models/soulspear/soulspear_diffuse.tga");
	//texNormal = loadTexture("../fbx_models/soulspear/soulspear_normal.tga");
	//texSpecular = loadTexture("../fbx_models/soulspear/soulspear_specular.tga");

	fbxData.loadFBX("../fbx_models/soulspear/soulspear.fbx");
	//std::cout << getMeshCount();


	camera.setPerspective(glm::pi<float>() * 0.5f, 9.f / 16.f, .5f, 4000);
	camera.setSpeed(5, 0.1f);

	Gizmos::create();

	setSky(vec3(0.1f, 0.1f, 0.1f));

	return true;
}
void AppFBX::onkill()
{
	ShaderLoader::freeShaderProgram(programID);
	fbxData.freeFBX();
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
	//Vertex positioning
	int projection = glGetUniformLocation(programID, "ProjectionView");
	int model = glGetUniformLocation(programID, "Model");
	//Ambient Light
	int ambientLight = glGetUniformLocation(programID, "AmbientLight");
	//Sun Light
	int lightDirection = glGetUniformLocation(programID, "LightDirection");
	int lightColor = glGetUniformLocation(programID, "LightColor");
	//Specular Reflection
	int cameraPosition = glGetUniformLocation(programID, "CameraPosition");
	int specularPower = glGetUniformLocation(programID, "SpecularPower");

	//VP
	glUniformMatrix4fv(projection, 1, false, value_ptr(camera.getProjectionView()));
	glUniformMatrix4fv(model, 1, false, value_ptr(rotate(getCurrentTime() / 5, vec3(0, -1, 0))) );
	//Ambient
	glUniform3fv(ambientLight, 1, glm::value_ptr(getSky()));
	//DL
	glUniform3fv(lightDirection, 1, glm::value_ptr( glm::normalize( glm::vec3(0, 1, 1) ) * 1.f ));
	glUniform3fv(lightColor, 1, glm::value_ptr(glm::vec3(1, 1, 1)));
	//SR
	glUniform3fv(cameraPosition, 1, glm::value_ptr(camera.getWorldTransform()[3]));
	glUniform1f(specularPower, 32.f);

	//textures
	int textureLoc = glGetUniformLocation(programID, "DiffuseMap");
	glUniform1i(textureLoc, 0);

	textureLoc = glGetUniformLocation(programID, "NormalMap");
	glUniform1i(textureLoc, 1);

	textureLoc = glGetUniformLocation(programID, "SpecularMap");
	glUniform1i(textureLoc, 2);
	
	//draw
	for (unsigned int i = 0; i < fbxData.glInfo.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, fbxData.glInfo[i].diffuse);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, fbxData.glInfo[i].normal);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, fbxData.glInfo[i].specular);


		glBindVertexArray(fbxData.glInfo[i].VAO);
		glDrawElements(GL_TRIANGLES, fbxData.glInfo[i].indexCount, GL_UNSIGNED_INT, 0);
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

/*..
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
		glEnableVertexAttribArray(3);

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)FBXVertex::Offsets::PositionOffset);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)FBXVertex::Offsets::NormalOffset);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)FBXVertex::Offsets::TangentOffset);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)FBXVertex::Offsets::TexCoord1Offset);

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
*/



/*
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
}*/