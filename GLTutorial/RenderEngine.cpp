#include "RenderEngine.h"

using namespace RenderEngine;

// Render Object ###################
void RenderObjectEx::instantiate()
{
	referenceID = Renderer::instance().createObject(this);
}

void RenderObjectEx::update()
{
	Renderer::instance().updateObject(this, referenceID);
}

void RenderObjectEx::destroy()
{
	Renderer::instance().clearObject(referenceID);
}

unsigned int RenderObjectEx::getReferenceID()
{
	return referenceID;
}
//internal
unsigned int Renderer::newObject()
{
	//Search for an object in vector tagged Unused
	for (unsigned i = 0; i < objectList.size(); i++)
	{
		if (objectList[i].inUse == false)
		{
			return i;
		}
	}
	return objectList.size() - 1;
}
unsigned int Renderer::createObject(RenderObjectEx* in)
{
	unsigned int slot = newObject();
	objectList[slot] = *in;
	objectList[slot].inUse = true;
	return slot;
}

void Renderer::updateObject(RenderObjectEx* in, unsigned int id)
{
	objectList[id] = *in;
}

void Renderer::clearObject(unsigned int id)
{
	objectList[id].inUse = false;
}




// Directional Light ###################
void DirectionalLightEx::instantiate()
{
	referenceID = Renderer::instance().createDLight(this);
}

void DirectionalLightEx::update()
{
	Renderer::instance().updateDLight(this, referenceID);
}

void DirectionalLightEx::destroy()
{
	Renderer::instance().clearDLight(referenceID);
}

unsigned int DirectionalLightEx::getReferenceID()
{
	return referenceID;
}
//internal
unsigned int Renderer::newDLight()
{
	//Search for an object in vector tagged Unused
	for (unsigned i = 0; i < directionalLightList.size(); i++)
	{
		if (directionalLightList[i].inUse == false)
		{
			return i;
		}
	}
	return directionalLightList.size() - 1;
}
unsigned int Renderer::createDLight(DirectionalLightEx* in)
{
	unsigned int slot = newDLight();
	directionalLightList[slot] = *in;
	directionalLightList[slot].inUse = true;
	return slot;
}

void Renderer::updateDLight(DirectionalLightEx* in, unsigned int id)
{
	directionalLightList[id] = *in;
}

void Renderer::clearDLight(unsigned int id)
{
	directionalLightList[id].inUse = false;
}




// Point Light ################
void PointLightEx::instantiate()
{
	referenceID = Renderer::instance().createPLight(this);
}

void PointLightEx::update()
{
	Renderer::instance().updatePLight(this, referenceID);
}

void PointLightEx::destroy()
{
	Renderer::instance().clearPLight(referenceID);
}

unsigned int PointLightEx::getReferenceID()
{
	return referenceID;
}
//internal
unsigned int Renderer::newPLight()
{
	//Search for an object in vector tagged Unused
	for (unsigned i = 0; i < pointLightList.size(); i++)
	{
		if (pointLightList[i].inUse == false)
		{
			return i;
		}
	}
	return pointLightList.size() - 1;
}
unsigned int Renderer::createPLight(PointLightEx* in)
{
	unsigned int slot = newPLight();
	pointLightList[slot] = *in;
	pointLightList[slot].inUse = true;
	return slot;
}

void Renderer::updatePLight(PointLightEx* in, unsigned int id)
{
	pointLightList[id] = *in;
}

void Renderer::clearPLight(unsigned int id)
{
	pointLightList[id].inUse = false;
}




//management
void Renderer::init()
{
	objectList.reserve(100);
	for (unsigned i = 0; i < 100; i++)
	{
		RenderObjectIn obj;
		objectList.push_back(obj);
	}
	directionalLightList.reserve(100);
	for (unsigned i = 0; i < 100; i++)
	{
		DirectionalLightIn obj;
		directionalLightList.push_back(obj);
	}
	pointLightList.reserve(100);
	for (unsigned i = 0; i < 100; i++)
	{
		PointLightIn obj;
		pointLightList.push_back(obj);
	}
	
	auto &a = AssetManager::instance();
	auto &w = Window::instance();
	//GPASS
	const char* gpasstexturenames[] = { "GPassAlbedo", "GPassPosition", "GPassNormal", "GPassSpecular" };
	const unsigned gpassdepths[] = { GL_RGB8, GL_RGB32F, GL_RGB32F, GL_RGBA32F };
	a.buildFBO("GPassFrameBuffer", w.getWidth(), w.getHeight(), 4, gpasstexturenames, gpassdepths, true);
	gPass.fbo = "GPassFrameBuffer";
	a.loadShader("GPassShader", "./DeferredRendering/GPass/GVertex.txt", "./DeferredRendering/GPass/GFragment.txt");
	gPass.shader = "GPassShader";
	//LIGHT
	const char* lighttexturenames[] = { "LPassLight", "LPassSpecular" };
	const unsigned lightdepths[] = { GL_RGB8, GL_RGB8 };
	a.buildFBO("LightFrameBuffer", w.getWidth(), w.getHeight(), 2, lighttexturenames, lightdepths, false);
	dLight.fbo = "LightFrameBuffer";
	pLight.fbo = "LightFrameBuffer";
	a.loadShader("DLightShader", "./DeferredRendering/Light/LightVertex.txt", "./DeferredRendering/Light/LightFragment.txt");
	dLight.shader = "DLightShader";
	a.loadShader("PLightShader", "./DeferredRendering/Light/PointLightVertex.txt", "./DeferredRendering/Light/PointLightFragment.txt");
	pLight.shader = "PLightShader";
	//COMP
	a.loadShader("CompositeShader", "./DeferredRendering/Composite/CompVertex.txt", "./DeferredRendering/Composite/CompFragment.txt");
	comp.shader = "CompositeShader";
}

void RenderEngine::Renderer::kill()
{
	objectList.clear();
	directionalLightList.clear();
	pointLightList.clear();
}

void RenderEngine::Renderer::setCamera(Camera c)
{
	camera.worldTransform = c.getWorldTransform();
	camera.projectionTransform = c.getProjection();
}

void Renderer::render()
{
	//gpass
	gPass.prep();
	for each (RenderObjectIn i in objectList)
	{
		if (i.inUse && i.visible)
		{
			gPass.draw(i, camera);
		}
	}
	gPass.post();
	//dlight
	dLight.prep();
	for each (DirectionalLightIn i in directionalLightList)
	{
		if (i.inUse && i.visible)
		{
			dLight.draw(i, camera);
		}
	}
	dLight.post();
	//plight
	pLight.prep();
	for each (PointLightIn i in pointLightList)
	{
		if (i.inUse && i.visible)
		{
			pLight.draw(i, camera);
		}
	}
	pLight.post();
	//comp
	comp.prep();
	comp.draw();
	comp.post();
}





void Renderer::GPassRender::prep()
{
	glBindFramebuffer(GL_FRAMEBUFFER, AssetManager::instance().get<ASSET::FBO>(fbo.name.c_str()));

	glEnable(GL_DEPTH_TEST);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::GPassRender::post()
{
	glDisable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);
}

void RenderEngine::Renderer::GPassRender::draw(RenderObjectIn ob, Camera c)
{
	glUseProgram(AssetManager::instance().get<ASSET::SHADER>(shader.name.c_str()));

	setUniform("ProjectionView", UNIFORM::MAT4, glm::value_ptr(c.getProjectionView()));
	setUniform("View", UNIFORM::MAT4, glm::value_ptr(c.getView()));

	setUniform("Model", UNIFORM::MAT4, glm::value_ptr(ob.transform.get()));

	setUniform("diffuseMap", UNIFORM::TEX2, ob.material.diffuseTexture, 0);
	setUniform("normalMap", UNIFORM::TEX2, ob.material.normalTexture, 1);
	setUniform("specularMap", UNIFORM::TEX2, ob.material.specularTexture, 2);

	glBindVertexArray(AssetManager::instance().get<ASSET::VAO>(ob.mesh.name.c_str()));
	glDrawElements(GL_TRIANGLES, AssetManager::instance().get<ASSET::SIZE>(ob.mesh.name.c_str()), GL_UNSIGNED_INT, 0);
}



void RenderEngine::Renderer::DLightPassRender::prep()
{
	glBindFramebuffer(GL_FRAMEBUFFER, AssetManager::instance().get<ASSET::FBO>(fbo.name.c_str()));

	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
}

void RenderEngine::Renderer::DLightPassRender::post()
{
	glDisable(GL_BLEND);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);
}

void RenderEngine::Renderer::DLightPassRender::draw(DirectionalLightIn li, Camera c)
{
	glUseProgram(AssetManager::instance().get<ASSET::SHADER>(shader.name.c_str()));

	setUniform("cameraView", UNIFORM::MAT4, glm::value_ptr(c.getView()));
	setUniform("cameraPosition", UNIFORM::FLO3, glm::value_ptr(c.getWorldTransform()[3]));

	setUniform("lightDirection", UNIFORM::FLO3, glm::value_ptr(li.direction));
	setUniform("lightDiffuse", UNIFORM::FLO3, glm::value_ptr(li.color));
	
	Asset<ASSET::TEXTURE> p;
	p = "GPassPosition";
	Asset<ASSET::TEXTURE> n;
	n = "GPassNormal";
	Asset<ASSET::TEXTURE> s;
	s = "GPassSpecular";

	setUniform("positionTexture", UNIFORM::TEX2, &p, 0);
	setUniform("normalTexture", UNIFORM::TEX2, &n, 1);
	setUniform("specularMap", UNIFORM::TEX2, &s, 2);

	glBindVertexArray(AssetManager::instance().get<ASSET::VAO>("Quad"));
	glDrawElements(GL_TRIANGLES, AssetManager::instance().get<ASSET::SIZE>("Quad"), GL_UNSIGNED_INT, 0);
}



void RenderEngine::Renderer::PLightPassRender::prep()
{
	glBindFramebuffer(GL_FRAMEBUFFER, AssetManager::instance().get<ASSET::FBO>(fbo.name.c_str()));

	//glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_FRONT);
}

void RenderEngine::Renderer::PLightPassRender::post()
{
	glDisable(GL_BLEND); 
	glDisable(GL_CULL_FACE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);
}

void RenderEngine::Renderer::PLightPassRender::draw(PointLightIn li, Camera c)
{
	glUseProgram(AssetManager::instance().get<ASSET::SHADER>(shader.name.c_str()));

	setUniform("ProjectionView", UNIFORM::MAT4, glm::value_ptr(c.getProjectionView()));
	setUniform("cameraView", UNIFORM::MAT4, glm::value_ptr(c.getView()));
	setUniform("cameraPosition", UNIFORM::FLO3, glm::value_ptr(c.getWorldTransform()[3]));

	setUniform("lightPositionView", UNIFORM::FLO3, glm::value_ptr(c.getView() * glm::vec4(li.position, 1)));

	setUniform("lightPosition", UNIFORM::FLO3, glm::value_ptr(li.position));
	setUniform("lightDiffuse", UNIFORM::FLO3, glm::value_ptr(li.color));
	setUniform("lightRadius", UNIFORM::FLO1, &li.radius);

	Asset<ASSET::TEXTURE> p;
	p = "GPassPosition";
	Asset<ASSET::TEXTURE> n;
	n = "GPassNormal";
	Asset<ASSET::TEXTURE> s;
	s = "GPassSpecular";

	setUniform("positionTexture", UNIFORM::TEX2, &p, 0);
	setUniform("normalTexture", UNIFORM::TEX2, &n, 1);
	setUniform("specularMap", UNIFORM::TEX2, &s, 2);

	glBindVertexArray(AssetManager::instance().get<ASSET::VAO>("Cube"));
	glDrawElements(GL_TRIANGLES, AssetManager::instance().get<ASSET::SIZE>("Cube"), GL_UNSIGNED_INT, 0);
}




void RenderEngine::Renderer::CompositePassRender::prep()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClear(GL_COLOR_BUFFER_BIT);

	
}

void RenderEngine::Renderer::CompositePassRender::post()
{
	
	glUseProgram(0);
}

void RenderEngine::Renderer::CompositePassRender::draw()
{
	glUseProgram(AssetManager::instance().get<ASSET::SHADER>(shader.name.c_str()));

	Asset<ASSET::TEXTURE> a;
	a = "GPassAlbedo";
	Asset<ASSET::TEXTURE> l;
	l = "LPassLight";
	Asset<ASSET::TEXTURE> s;
	s = "GPassSpecular";
	Asset<ASSET::TEXTURE> ls;
	ls = "LPassSpecular";

	setUniform("albedoTexture", UNIFORM::TEX2, &a, 0);
	setUniform("lightTexture", UNIFORM::TEX2, &l, 1);
	setUniform("specularTexture", UNIFORM::TEX2, &s, 2);
	setUniform("specularTexture", UNIFORM::TEX2, &ls, 3);


	glBindVertexArray(AssetManager::instance().get<ASSET::VAO>("Quad"));
	glDrawElements(GL_TRIANGLES, AssetManager::instance().get<ASSET::SIZE>("Quad"), GL_UNSIGNED_INT, 0);
}
