#include "AppDeferred.h"

AppDeferred::AppDeferred()
{

}

int AppDeferred::oninit()
{
	//gen fbo
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	//gen texture
	glGenTextures(1, &FBOTexture);
	glBindTexture(GL_TEXTURE_2D, FBOTexture);
	//texture things
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, 512*2, 512*2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//attach texture
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, FBOTexture, 0);
	//gen render buffer
	glGenRenderbuffers(1, &FBODepth);
	glBindRenderbuffer(GL_RENDERBUFFER, FBODepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512*2, 512*2);
	//render buffer attach
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, FBODepth);

	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);
	//status check
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Framebuffer Error!\n");
	}
	//unbind fbo
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	quad.create();
	quad.texture = FBOTexture;


	FBXShader = ShaderLoader::createShaderProgram("./VertexShader.txt", "./FragmentShader.txt");
	fbxData.loadFBX("../fbx_models/soulspear/soulspear.fbx");

	camera.setPerspective(glm::pi<float>() * 0.5f, 9.f / 16.f, .5f, 4000);
	camera.setSpeed(5, 0.1f);

	Gizmos::create();
	setSky(vec3(0.1f, 0.1f, 0.1f));

	return true;
}
void AppDeferred::onkill()
{
	ShaderLoader::freeShaderProgram(FBXShader);
	fbxData.freeFBX();
	Gizmos::destroy();
}

bool AppDeferred::onstep(float deltaTime)
{
	camera.update(deltaTime);
	return true;
}
void AppDeferred::ondraw()
{
	//draw to frame buffer 1111111111
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glViewport(0, 0, 512*2, 512*2);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera.setPerspective(glm::pi<float>() * 0.5f, 1.f / 1.f, .5f, 4000);

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
	FBXDraw();

	//draw normal 2222222222222
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, getWidth(), getHeight());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera.setPerspective(glm::pi<float>() * 0.5f, 9.f / 16.f, .5f, 4000);
	//framebuffer quad
	quad.draw(camera.getProjectionView());
	FBXDraw();

	//Gizmos ---------------------------
	Gizmos::clear();
	Gizmos::addTransform(glm::mat4(1));
	//vec4 white(1);
	//vec4 black(0, 0, 0, 1);

	for (int i = 0; i < 21; ++i)
	{
		Gizmos::addLine(vec3(-10 + i, 0, 10), vec3(-10 + i, 0, -10), i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i), vec3(-10, 0, -10 + i), i == 10 ? white : black);
	}
	Gizmos::draw(camera.getProjectionView());
}

void AppDeferred::FBXDraw()
{
	//program
	glUseProgram(FBXShader);
	//Vertex positioning
	int projection = glGetUniformLocation(FBXShader, "ProjectionView");
	int model = glGetUniformLocation(FBXShader, "Model");
	//Ambient Light
	int ambientLight = glGetUniformLocation(FBXShader, "AmbientLight");
	//Sun Light
	int lightDirection = glGetUniformLocation(FBXShader, "LightDirection");
	int lightColor = glGetUniformLocation(FBXShader, "LightColor");
	//Specular Reflection
	int cameraPosition = glGetUniformLocation(FBXShader, "CameraPosition");
	int specularPower = glGetUniformLocation(FBXShader, "SpecularPower");

	//VP
	glUniformMatrix4fv(projection, 1, false, value_ptr(camera.getProjectionView()));
	glUniformMatrix4fv(model, 1, false, value_ptr(rotate(getCurrentTime() / 5, vec3(0, -1, 0))));
	//Ambient
	glUniform3fv(ambientLight, 1, glm::value_ptr(getSky()));
	//DL
	glUniform3fv(lightDirection, 1, glm::value_ptr(glm::normalize(glm::vec3(0, 1, 1)) * 1.f));
	glUniform3fv(lightColor, 1, glm::value_ptr(glm::vec3(1, 1, 1)));
	//SR
	glUniform3fv(cameraPosition, 1, glm::value_ptr(camera.getWorldTransform()[3]));
	glUniform1f(specularPower, 32.f);

	//textures
	int textureLoc = glGetUniformLocation(FBXShader, "DiffuseMap");
	glUniform1i(textureLoc, 0);

	textureLoc = glGetUniformLocation(FBXShader, "NormalMap");
	glUniform1i(textureLoc, 1);

	textureLoc = glGetUniformLocation(FBXShader, "SpecularMap");
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

	glBindVertexArray(0);
	glUseProgram(0);
}