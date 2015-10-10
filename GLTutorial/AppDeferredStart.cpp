#include "AppDeferredStart.h"

AppDeferredStart::AppDeferredStart() {}

int AppDeferredStart::oninit()
{
	GProgram = ShaderLoader::createShaderProgram("./DeferredRendering/GPass/GVertex.txt", "./DeferredRendering/GPass/GFragment.txt");
	compProgram = ShaderLoader::createShaderProgram("./DeferredRendering/Composite/CompVertex.txt", "./DeferredRendering/Composite/CompFragment.txt");
	lightProgram = ShaderLoader::createShaderProgram("./DeferredRendering/Light/LightVertex.txt", "./DeferredRendering/Light/LightFragment.txt");
	pointLightProgram = ShaderLoader::createShaderProgram("./DeferredRendering/Light/PointLightVertex.txt", "./DeferredRendering/Light/PointLightFragment.txt");
	gPass.create(getWidth(), getHeight());
	lightPass.create(getWidth(), getHeight());
	lightQuad.create(getWidth(), getHeight());
	pointLightCube.create();
	compositeQuad.create(getWidth(), getHeight());

	testQuad.create(getWidth(), getHeight(), 0.3f);
	testQuad2.create(getWidth(), getHeight(), 0.15f);

	fbxData.loadFBX("../fbx_models/soulspear/soulspear.fbx");
	camera.setPerspective(glm::pi<float>() * 0.5f, 9.f / 16.f, .5f, 200);
	camera.setSpeed(5, 0.1f);

	Gizmos::create();
	setSky(vec3(0));

	return true;
}
void AppDeferredStart::onkill()
{
	ShaderLoader::freeShaderProgram(GProgram);
	ShaderLoader::freeShaderProgram(compProgram);
	ShaderLoader::freeShaderProgram(lightProgram);
	ShaderLoader::freeShaderProgram(pointLightProgram);
}

bool AppDeferredStart::onstep(float deltaTime)
{
	camera.update(deltaTime);

	return true;
}
void AppDeferredStart::ondraw()
{
	// G-Pass: render out the albedo, position and normal
	//----------------------------------------------------
	glEnable(GL_DEPTH_TEST);

	glBindFramebuffer(GL_FRAMEBUFFER, gPass.FBO);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(GProgram);
	// bind camera transforms
	int loc = glGetUniformLocation(GProgram, "ProjectionView");
	glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(camera.getProjectionView()));

	loc = glGetUniformLocation(GProgram, "View");
	glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(camera.getView()));

	int textureLoc = glGetUniformLocation(GProgram, "diffuseMap");
	glUniform1i(textureLoc, 0);

	textureLoc = glGetUniformLocation(GProgram, "normalMap");
	glUniform1i(textureLoc, 1);

	textureLoc = glGetUniformLocation(GProgram, "specularMap");
	glUniform1i(textureLoc, 2);
	// draw our scene
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

	// Light Pass: render lights as geometry, sampling position and
	// normals disable depth testing and enable additive blending
	//----------------------------------------------------
	glBindFramebuffer(GL_FRAMEBUFFER, lightPass.FBO);
	glClear(GL_COLOR_BUFFER_BIT);

	glDisable(GL_DEPTH_TEST);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	//Directional Light ###############
	glUseProgram(lightProgram);

	loc = glGetUniformLocation(lightProgram, "cameraPosition");
	glUniform3fv(loc, 1, glm::value_ptr(camera.getWorldTransform()[3]));

	loc = glGetUniformLocation(lightProgram, "positionTexture");
	glUniform1i(loc, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gPass.positionTexture);

	loc = glGetUniformLocation(lightProgram, "normalTexture");
	glUniform1i(loc, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gPass.normalTexture);

	loc = glGetUniformLocation(lightProgram, "specularMap");
	glUniform1i(loc, 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gPass.specularMap);
	// draw Directional lights as fullscreen quads

//	drawDirectionalLight(glm::vec3(0, 1, 1), glm::vec3(1.0f));
	//drawDirectionalLight(glm::vec3(0, -1, 0), glm::vec3(0, 0, 0.2f));

	//Finish Directional Lights

	//Point Lights ##################
	glUseProgram(pointLightProgram);

	loc = glGetUniformLocation(pointLightProgram, "ProjectionView");
	glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(camera.getProjectionView()));

	loc = glGetUniformLocation(pointLightProgram, "cameraPosition");
	glUniform3fv(loc, 1, glm::value_ptr(camera.getWorldTransform()[3]));

	loc = glGetUniformLocation(pointLightProgram, "positionTexture");
	glUniform1i(loc, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gPass.positionTexture);

	loc = glGetUniformLocation(pointLightProgram,"normalTexture");
	glUniform1i(loc, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gPass.normalTexture);

	loc = glGetUniformLocation(pointLightProgram, "specularMap");
	glUniform1i(loc, 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gPass.specularMap);

	//drawPoint Lights
	//-------------
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_FRONT);

	float t = (float)glfwGetTime();
	//drawPointLight(glm::vec3(0, 6, 0), 6, glm::vec3(0, 1, 0));
//	drawPointLight(glm::vec3(sin(t) * 2, 2, cos(t) * 2), 5, glm::vec3(1.0f));
	drawPointLight(glm::vec3(2, 2, 2), 10, glm::vec3(1.0f));

	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);
	//Finish Point Lights
	glDisable(GL_BLEND);
	//Finish Light Pass

	// Composite Pass: render a quad and combine albedo and light
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(compProgram);

	loc = glGetUniformLocation(compProgram, "albedoTexture");
	glUniform1i(loc, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gPass.albedoTexture);

	loc = glGetUniformLocation(compProgram, "lightTexture");
	glUniform1i(loc, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, lightPass.lightTexture);

	loc = glGetUniformLocation(compProgram, "specularTexture");
	glUniform1i(loc, 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gPass.specularMap);

	loc = glGetUniformLocation(compProgram, "specularity");
	glUniform1i(loc, 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, lightPass.specularTexture);

	compositeQuad.draw();

	testQuad.texture = lightPass.specularTexture;
	testQuad.draw();
	testQuad2.texture = lightPass.lightTexture;
	testQuad2.draw();
}

void AppDeferredStart::drawDirectionalLight(const vec3 direction, const vec3 diffuse)
{
	glm::vec4 viewSpaceLight = camera.getView() * glm::vec4(glm::normalize(direction), 0);

	int loc = glGetUniformLocation(lightProgram, "lightDirection");
	glUniform3fv(loc, 1, value_ptr(viewSpaceLight));

	loc = glGetUniformLocation(lightProgram, "lightDiffuse");
	glUniform3fv(loc, 1, value_ptr(diffuse));

	//glBindVertexArray(lightQuad.VAO);
	//glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0);
	lightQuad.draw();
}

void AppDeferredStart::drawPointLight(glm::vec3 position, float radius, glm::vec3 color)
{
	glm::vec4 viewSpacePosition = camera.getView() * glm::vec4(position, 1);

	int loc = glGetUniformLocation(pointLightProgram, "lightPosition");
	glUniform3fv(loc, 1, value_ptr(position));

	loc = glGetUniformLocation(pointLightProgram, "lightPositionView");
	glUniform3fv(loc, 1, value_ptr(viewSpacePosition));

	loc = glGetUniformLocation(pointLightProgram, "lightRadius");
	glUniform1f(loc, radius);

	loc = glGetUniformLocation(pointLightProgram, "lightDiffuse");
	glUniform3fv(loc, 1, value_ptr(color));

	pointLightCube.draw();
}