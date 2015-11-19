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



// Particle Emitter ################
void ParticleEmitterEx::instantiate()
{
	referenceID = Renderer::instance().createEmitter(this);
}

void ParticleEmitterEx::update()
{
	Renderer::instance().updateEmitter(this, referenceID);
}

void ParticleEmitterEx::destroy()
{
	Renderer::instance().clearEmitter(referenceID);
}

unsigned int ParticleEmitterEx::getReferenceID()
{
	return referenceID;
}
//internal
unsigned int Renderer::newEmitter()
{
	//Search for an object in vector tagged Unused
	for (unsigned i = 0; i < emitterList.size(); i++)
	{
		if (emitterList[i].inUse == false)
		{
			return i;
		}
	}
	return emitterList.size() - 1;
}
unsigned int Renderer::createEmitter(ParticleEmitterEx* in)
{
	unsigned int slot = newEmitter();
	emitterList[slot] = *in;
	emitterList[slot].inUse = true;
	return slot;
}

void Renderer::updateEmitter(ParticleEmitterEx* in, unsigned int id)
{
	emitterList[id] = *in;
}

void Renderer::clearEmitter(unsigned int id)
{
	emitterList[id].inUse = false;
}



// Rendering ##########################################
void Renderer::bindShader(unsigned int shader)
{
	boundShader = shader;
	glUseProgram(boundShader);
}
bool Renderer::setUniform(const char * name, UNIFORM::TYPE type, const void * value, unsigned count, bool normalize)
{
	int loc = glGetUniformLocation(boundShader, name);
	switch (type)
	{
	case UNIFORM::FLO1: glUniform1fv(loc, count, (GLfloat*)value); break;
	case UNIFORM::FLO2: glUniform2fv(loc, count, (GLfloat*)value); break;
	case UNIFORM::FLO3: glUniform3fv(loc, count, (GLfloat*)value); break;
	case UNIFORM::FLO4: glUniform4fv(loc, count, (GLfloat*)value); break;
	case UNIFORM::MAT4: glUniformMatrix4fv(loc, count, normalize, (GLfloat*)value); break;
	case UNIFORM::INT1: glUniform1i(loc, (GLint)value); break;
	case UNIFORM::TEX2: glActiveTexture(GL_TEXTURE0 + count);
		glBindTexture(GL_TEXTURE_2D, *(const GLuint*)value);
		glUniform1i(loc, count);
		break;
	default: return false;
	}

	return true;
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
	emitterList.reserve(5);
	for (unsigned i = 0; i < 5; i++)
	{
		ParticleEmitterIn obj;
		emitterList.push_back(obj);
	}
	
	auto &a = AssetManager::instance();
	auto &w = Window::instance();
	//GPASS
	const char* gpasstexturenames[] = { "GPassAlbedo", "GPassPosition", "GPassNormal", "GPassSpecular", "GPassGlow" };
	const unsigned gpassdepths[] = { GL_RGB8, GL_RGB32F, GL_RGB32F, GL_RGBA32F, GL_RGB32F };
	a.buildFBO("GPassFrameBuffer", w.getWidth(), w.getHeight(), 5, gpasstexturenames, gpassdepths, true);
	//gPass.fbo = "GPassFrameBuffer";
	a.loadShader("GPassShader", "./DeferredRendering/GPass/GVertex.txt", "./DeferredRendering/GPass/GFragment.txt");
	//gPass.shader = "GPassShader";

	//SHADOW
	unsigned int buffer;
	glGenFramebuffers(1, &buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, buffer);

	//a.buildTexture("ShadowMap", 1024 * 4, 1024 * 4, GL_DEPTH_COMPONENT16);
	unsigned int tbuffer;
	glGenTextures(1, &tbuffer);
	glBindTexture(GL_TEXTURE_2D, tbuffer);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT16, 1024 * 16, 1024 * 16);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	a.manualAsset("ShadowMap", ASSET::TEXTURE, tbuffer);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, AssetManager::instance().get<ASSET::TEXTURE>("ShadowMap"), 0);
	
	glDrawBuffer(GL_NONE);
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	a.manualAsset("ShadowFrameBuffer", ASSET::FBO, buffer);
	a.loadShader("ShadowShader", "./DeferredRendering/Light/ShadowVertex.txt", "./DeferredRendering/Light/ShadowFragment.txt");

	//LIGHT
	const char* lighttexturenames[] = { "LPassLight", "LPassSpecular" };
	const unsigned lightdepths[] = { GL_RGB8, GL_RGB8 };
	a.buildFBO("LightFrameBuffer", w.getWidth(), w.getHeight(), 2, lighttexturenames, lightdepths, false);
	//dLight.fbo = "LightFrameBuffer";
	//pLight.fbo = "LightFrameBuffer";
	a.loadShader("DLightShader", "./DeferredRendering/Light/LightVertex.txt", "./DeferredRendering/Light/LightFragment.txt");
	//dLight.shader = "DLightShader";
	a.loadShader("PLightShader", "./DeferredRendering/Light/PointLightVertex.txt", "./DeferredRendering/Light/PointLightFragment.txt");
	//pLight.shader = "PLightShader";

	//COMP
	a.loadShader("CompositeShader", "./DeferredRendering/Composite/CompVertex.txt", "./DeferredRendering/Composite/CompFragment.txt");
	//comp.shader = "CompositeShader";
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

//RENDERING #$#$#$#$#$#$#$#$#$#$#$#
void Renderer::render(float deltaTime)
{
	//gpass ########################
	gPass.prep();
	
	bindShader(AssetManager::instance().get<ASSET::SHADER>("GPassShader"));

	setUniform("ProjectionView", UNIFORM::MAT4, glm::value_ptr(camera.getProjectionView()));
	setUniform("View", UNIFORM::MAT4, glm::value_ptr(camera.getView()));

	setUniform("ambientLight", UNIFORM::FLO3, glm::value_ptr(ambientLight));

	for each (RenderObjectIn i in objectList)
	{
		if (i.inUse && i.visible)
		{
			setUniform("Model", UNIFORM::MAT4, glm::value_ptr(i.transform.get()));

			setUniform("specPower", UNIFORM::FLO1, &i.material.specularPower);

			setUniform("diffuseTint", UNIFORM::FLO3, glm::value_ptr(i.material.diffuseTint));
			setUniform("specularTint", UNIFORM::FLO3, glm::value_ptr(i.material.specularTint));
			setUniform("glowTint", UNIFORM::FLO3, glm::value_ptr(i.material.glowTint));

			setUniform("diffuseMap", UNIFORM::TEX2, i.material.diffuseTexture, 0);
			setUniform("normalMap", UNIFORM::TEX2, i.material.normalTexture, 1);
			setUniform("specularMap", UNIFORM::TEX2, i.material.specularTexture, 2);
			setUniform("glowMap", UNIFORM::TEX2, i.material.glowTexture, 3);

			glBindVertexArray(AssetManager::instance().get<ASSET::VAO>(i.mesh.name.c_str()));
			glDrawElements(GL_TRIANGLES, AssetManager::instance().get<ASSET::SIZE>(i.mesh.name.c_str()), GL_UNSIGNED_INT, 0);
		}
	}
	// PARTICLES
	for (unsigned i = 0; i < 5; i++)
	{
		if (emitterList[i].inUse)
		{
			emitterList[i].update(deltaTime);
			for (unsigned j = 0; j < 50; j++)
			{
				if (emitterList[i].particles[j].live)
				{
					setUniform("Model", UNIFORM::MAT4, glm::value_ptr(emitterList[i].particles[j].trans.get()));

					float zero = 0;
					setUniform("specPower", UNIFORM::FLO1, &zero);

					setUniform("diffuseTint", UNIFORM::FLO3, glm::value_ptr(emitterList[i].particles[j].color));
					setUniform("specularTint", UNIFORM::FLO3, glm::value_ptr(glm::vec3(0)));
					setUniform("glowTint", UNIFORM::FLO3, glm::value_ptr(glm::vec3(0)));

					Asset<ASSET::TEXTURE> d;
					d = "Black";
					setUniform("diffuseMap", UNIFORM::TEX2, &d, 0);
					Asset<ASSET::TEXTURE> n;
					n = "Flat";
					setUniform("normalMap", UNIFORM::TEX2, &n, 1);
					Asset<ASSET::TEXTURE> s;
					s = "Black";
					setUniform("specularMap", UNIFORM::TEX2, &s, 2);
					Asset<ASSET::TEXTURE> g;
					g = "Black";
					setUniform("glowMap", UNIFORM::TEX2, &g, 3);

					glBindVertexArray(AssetManager::instance().get<ASSET::VAO>("Cube"));
					glDrawElements(GL_TRIANGLES, AssetManager::instance().get<ASSET::SIZE>("Cube"), GL_UNSIGNED_INT, 0);
				}
			}
		}
	}

	gPass.post();

	//PRE LIGHT CLEAR
	glBindFramebuffer(GL_FRAMEBUFFER, AssetManager::instance().get<ASSET::FBO>("LightFrameBuffer"));
	glClear(GL_COLOR_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Directional Light ########################
	for each (DirectionalLightIn i in directionalLightList)
	{
		if (i.inUse && i.visible)
		{
			//shadow %%%%%%%%
/*prep*/	glm::vec3 m_lightDirection = glm::normalize(i.direction);
			float projSize = 200;
			glm::mat4 lightProjection = glm::ortho<float>(-projSize, projSize, -projSize, projSize, -projSize, projSize);
			glm::mat4 lightView = glm::lookAt(glm::vec3(camera.getView()[3]), glm::vec3(camera.getView()[3]) - m_lightDirection, glm::vec3(0, 1, 0));
			//lightView[3].x = camera.getView()[3].x; lightView[3].y = camera.getView()[3].y; lightView[3].z = camera.getView()[3].z;
			glm::mat4 m_lightMatrix = lightProjection * lightView;
			glBindFramebuffer(GL_FRAMEBUFFER, AssetManager::instance().get<ASSET::FBO>("ShadowFrameBuffer"));

			glEnable(GL_DEPTH_TEST);

			glEnable(GL_CULL_FACE);
			glFrontFace(GL_CCW);
			glCullFace(GL_FRONT);

			glViewport(0, 0, 1024 * 16, 1024 * 16);
			glClear(GL_DEPTH_BUFFER_BIT);

			bindShader(AssetManager::instance().get<ASSET::SHADER>("ShadowShader"));
			setUniform("lightMatrix", UNIFORM::MAT4, glm::value_ptr(m_lightMatrix));

/*draw*/	for each (RenderObjectIn j in objectList)
			{
				if (j.inUse && j.visible)
				{
					setUniform("Model", UNIFORM::MAT4, glm::value_ptr(j.transform.get()));

					glBindVertexArray(AssetManager::instance().get<ASSET::VAO>(j.mesh.name.c_str()));
					glDrawElements(GL_TRIANGLES, AssetManager::instance().get<ASSET::SIZE>(j.mesh.name.c_str()), GL_UNSIGNED_INT, 0);
				}
			}

/*post*/	glViewport(0, 0, Window::instance().getWidth(), Window::instance().getHeight());
			glDisable(GL_DEPTH_TEST);
			glFrontFace(GL_CCW);
			glDisable(GL_CULL_FACE);


			//light %%%%%%%%
			dLight.prep();

			bindShader(AssetManager::instance().get<ASSET::SHADER>("DLightShader"));

			setUniform("cameraView", UNIFORM::MAT4, glm::value_ptr(camera.getView()));

			glm::vec3 d = glm::vec3(camera.getWorldTransform()[3]);
			//std::cout << d.x << " " << d.y << " " << d.z << std::endl;
			setUniform("cameraPosition", UNIFORM::FLO3, glm::value_ptr(d));
		
			setUniform("lightDirection", UNIFORM::FLO3, glm::value_ptr(i.direction));
			setUniform("lightDiffuse", UNIFORM::FLO3, glm::value_ptr(i.color));

			setUniform("lightMatrix", UNIFORM::MAT4, glm::value_ptr(m_lightMatrix));

			Asset<ASSET::TEXTURE> p;
			p = "GPassPosition";
			Asset<ASSET::TEXTURE> n;
			n = "GPassNormal";
			Asset<ASSET::TEXTURE> s;
			s = "GPassSpecular";
			Asset<ASSET::TEXTURE> h;
			h = "ShadowMap";

			setUniform("positionTexture", UNIFORM::TEX2, &p, 0);
			setUniform("normalTexture", UNIFORM::TEX2, &n, 1);
			setUniform("specularMap", UNIFORM::TEX2, &s, 2);
			setUniform("shadowMap", UNIFORM::TEX2, &h, 3);

			glBindVertexArray(AssetManager::instance().get<ASSET::VAO>("Quad"));
			glDrawElements(GL_TRIANGLES, AssetManager::instance().get<ASSET::SIZE>("Quad"), GL_UNSIGNED_INT, 0);
		}
	}

	dLight.post();


	//plight ########################
	pLight.prep();
	
	bindShader(AssetManager::instance().get<ASSET::SHADER>("PLightShader"));

	for each (PointLightIn i in pointLightList)
	{
		if (i.inUse && i.visible)
		{
			setUniform("ProjectionView", UNIFORM::MAT4, glm::value_ptr(camera.getProjectionView()));
			setUniform("cameraView", UNIFORM::MAT4, glm::value_ptr(camera.getView()));
			setUniform("cameraPosition", UNIFORM::FLO3, glm::value_ptr(camera.getWorldTransform()[3]));

			setUniform("lightPositionView", UNIFORM::FLO3, glm::value_ptr(camera.getView() * glm::vec4(i.position, 1)));

			setUniform("lightPosition", UNIFORM::FLO3, glm::value_ptr(i.position));
			setUniform("lightDiffuse", UNIFORM::FLO3, glm::value_ptr(i.color));
			setUniform("lightRadius", UNIFORM::FLO1, &i.radius);

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
	}

	pLight.post();


	//comp ########################
	comp.prep();
	//comp.draw();
	bindShader(AssetManager::instance().get<ASSET::SHADER>("CompositeShader"));

	Asset<ASSET::TEXTURE> g;
	g = "GPassGlow";
	Asset<ASSET::TEXTURE> a;
	a = "GPassAlbedo";
	//a = "GPassNormal";
	Asset<ASSET::TEXTURE> l;
	l = "LPassLight";
	Asset<ASSET::TEXTURE> s;
	s = "GPassSpecular";
	Asset<ASSET::TEXTURE> ls;
	ls = "LPassSpecular";

	setUniform("glowTexture", UNIFORM::TEX2, &g, 0);
	setUniform("albedoTexture", UNIFORM::TEX2, &a, 1);
	setUniform("lightTexture", UNIFORM::TEX2, &l, 2);
	setUniform("specularTexture", UNIFORM::TEX2, &s, 3);
	setUniform("specularity", UNIFORM::TEX2, &ls, 4);


	glBindVertexArray(AssetManager::instance().get<ASSET::VAO>("Quad"));
	glDrawElements(GL_TRIANGLES, AssetManager::instance().get<ASSET::SIZE>("Quad"), GL_UNSIGNED_INT, 0);

	comp.post();
}





void Renderer::GPassRender::prep()
{
	glBindFramebuffer(GL_FRAMEBUFFER, AssetManager::instance().get<ASSET::FBO>("GPassFrameBuffer"));

	glEnable(GL_DEPTH_TEST);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::GPassRender::post()
{
	glDisable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);
}

/*void RenderEngine::Renderer::GPassRender::draw(RenderObjectIn ob, Camera c)
{
	glUseProgram(AssetManager::instance().get<ASSET::SHADER>(shader.name.c_str()));

	setUniform("ProjectionView", UNIFORM::MAT4, glm::value_ptr(c.getProjectionView()));
	setUniform("View", UNIFORM::MAT4, glm::value_ptr(c.getView()));

	setUniform("Model", UNIFORM::MAT4, glm::value_ptr(ob.transform.get()));

	setUniform("specPower", UNIFORM::FLO1, &ob.material.specularPower);

	setUniform("diffuseMap", UNIFORM::TEX2, ob.material.diffuseTexture, 0);
	setUniform("normalMap", UNIFORM::TEX2, ob.material.normalTexture, 1);
	setUniform("specularMap", UNIFORM::TEX2, ob.material.specularTexture, 2);

	glBindVertexArray(AssetManager::instance().get<ASSET::VAO>(ob.mesh.name.c_str()));
	glDrawElements(GL_TRIANGLES, AssetManager::instance().get<ASSET::SIZE>(ob.mesh.name.c_str()), GL_UNSIGNED_INT, 0);
	
}*/





void RenderEngine::Renderer::DLightPassRender::prep()
{
	glBindFramebuffer(GL_FRAMEBUFFER, AssetManager::instance().get<ASSET::FBO>("LightFrameBuffer"));

	//glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
}

void RenderEngine::Renderer::DLightPassRender::post()
{
	glDisable(GL_BLEND);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);
}

/*void RenderEngine::Renderer::DLightPassRender::draw(DirectionalLightIn li, Camera c)
{
	//Do Shadow here

	//glUseProgram(AssetManager::instance().get<ASSET::SHADER>(shader.name.c_str()));

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
}*/





void RenderEngine::Renderer::PLightPassRender::prep()
{
	glBindFramebuffer(GL_FRAMEBUFFER, AssetManager::instance().get<ASSET::FBO>("LightFrameBuffer"));

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

/*void RenderEngine::Renderer::PLightPassRender::draw(PointLightIn li, Camera c)
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
}*/




void RenderEngine::Renderer::CompositePassRender::prep()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClear(GL_COLOR_BUFFER_BIT);

	
}

void RenderEngine::Renderer::CompositePassRender::post()
{
	
	glUseProgram(0);
}

/*void RenderEngine::Renderer::CompositePassRender::draw()
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
}*/
