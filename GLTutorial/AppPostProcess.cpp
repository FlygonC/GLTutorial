#include "AppPostProcess.h"

AppPostProcess::AppPostProcess() {}

int AppPostProcess::oninit()
{
	FBO.create(getWidth(), getHeight());
	screen.create(getWidth(), getHeight());

	FBXShader = ShaderLoader::createShaderProgram("./VertexShader.txt", "./FragmentShader.txt");
	fbxData.loadFBX("../fbx_models/soulspear/soulspear.fbx");

	camera.setPerspective(glm::pi<float>() * 0.5f, 9.f / 16.f, .5f, 1000);
	camera.setSpeed(5, 0.1f);

	sky.create(16.f, 9.f);
	sky.loadTexture("../resources/skyimagetest.png");

	box.create();
	box.loadCubeMap("../resources/sky1/lagoon_rt.tga",
		"../resources/sky1/lagoon_lf.tga",
		"../resources/sky1/lagoon_up.tga",
		"../resources/sky1/lagoon_dn.tga",
		"../resources/sky1/lagoon_bk.tga",
		"../resources/sky1/lagoon_ft.tga");

	Gizmos::create();
	setSky(vec3(0.5f, 0.5f, 0.5f));

	return true;
}

void AppPostProcess::onkill()
{
	Gizmos::destroy();
	glDeleteShader(FBXShader);
	fbxData.freeFBX();
}

bool AppPostProcess::onstep(float deltaTime)
{
	camera.update(deltaTime);
	return true;
}

void AppPostProcess::ondraw()
{
	// bind our target
	glBindFramebuffer(GL_FRAMEBUFFER, FBO.frame);
	glViewport(0, 0, getWidth(), getHeight());
	// clear the target
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// draw our 3D scene
	//sky.draw(camera.rotationX, camera.rotationY);
	box.draw(camera);
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



	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, getWidth(), getHeight());
	// just clear the back-buffer depth as
	// each pixel will be filled
	glClear(GL_DEPTH_BUFFER_BIT);
	// draw out full-screen quad
	screen.texture = FBO.texture;
	screen.draw();
}

void AppPostProcess::FBXDraw()
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
	glUniform3fv(lightDirection, 1, glm::value_ptr(glm::normalize(glm::vec3(1.f, 0.6f, -1.f))));
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