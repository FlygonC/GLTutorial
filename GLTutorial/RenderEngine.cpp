#include "RenderEngine.h"

using namespace RenderEngine;

RenderObjectEx::RenderObjectEx(DATATYPE::TYPE type)
{
	dataType = type;
	switch (type)
	{
	case DATATYPE::GEOMETRY:
		//data = new GeometryData();
		break;
	case DATATYPE::DIRECTIONALLIGHT:
		//data = new DirectionalLightData();
		break;
	case DATATYPE::POINTLIGHT:
		//data = new PointLightData();
		break;
	case DATATYPE::PARTICLEEMITTER:
		//data = new ParticleEmitterData();
		break;
	}
	tagNumber = Renderer::instance().newObject(type, data);
}
RenderObjectEx::~RenderObjectEx()
{
	Renderer::instance().clearObject(tagNumber);
	//delete data;
}
void RenderObjectEx::update()
{
	Renderer::instance().updateObject(tagNumber, data);
}
void RenderObjectEx::destroy(unsigned int tag)
{

}

bool Renderer::compare(const DATATYPE::TYPE &l, const DATATYPE::TYPE &r)
{
	return l < r;
}
unsigned int Renderer::newObject(DATATYPE::TYPE type, DataContainer in)
{
	unsigned int nextEmpty = typeCounts[0] + typeCounts[1] + typeCounts[2] + typeCounts[3];

	//RenderObjectIn newObject;
	//*newObject.data = *in;
	//newObject.dataType = type;
	//newObject.inUse = true;

	//newObject.tagNumber = objectCount;
	objectCount++;

	//objectList[nextEmpty] = newObject;
	std::sort(objectList.begin(), objectList.end(), compare);

	typeCounts[type]++;
	return objectCount - 1;;
}
void Renderer::updateObject(unsigned int tag, DataContainer in)
{
	for (unsigned i = 0; i < objectList.size(); i++)
	{
		if (objectList[i].tagNumber == tag)
		{
			*objectList[i].data = *in;
			return;
		}
	}
}
void Renderer::clearObject(unsigned int tag)
{
	for (unsigned i = 0; i < objectList.size(); i++)
	{
		if (objectList[i].tagNumber == tag)
		{
			typeCounts[objectList[i].dataType]--;
			objectList[i].clear();
			return;
		}
	}
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
/*	directionalLightList.reserve(100);
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
	}*/
	
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
	
}

void RenderEngine::Renderer::setCamera(Camera c)
{
	camera.worldTransform = c.getWorldTransform();
	camera.projectionTransform = c.getProjection();
}

//RENDERING #$#$#$#$#$#$#$#$#$#$#$#
void Renderer::render(float deltaTime)
{
	unsigned int dLightStart =  typeCounts[0] + 1;
	unsigned int pLightStart =  typeCounts[0] + typeCounts[1] + 1;
	unsigned int emitterStart = typeCounts[0] + typeCounts[1] + typeCounts[2] + 1;
	//gpass ########################
	gPass.prep();

	bindShader(AssetManager::instance().get<ASSET::SHADER>("GPassShader"));

	setUniform("ProjectionView", UNIFORM::MAT4, glm::value_ptr(camera.getProjectionView()));
	setUniform("View", UNIFORM::MAT4, glm::value_ptr(camera.getView()));

	setUniform("ambientLight", UNIFORM::FLO3, glm::value_ptr(ambientLight));

	for (unsigned i = 0; i < dLightStart; i++)
	{
		if (objectList[i].inUse && objectList[i].data.visible)
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
			emitterList[i].update(deltaTime, camera.getView());
			glBindVertexArray(AssetManager::instance().get<ASSET::VAO>("Quad"));
			float zero = 0;
			setUniform("specPower", UNIFORM::FLO1, &zero);
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

			for (unsigned j = 0; j < 50; j++)
			{
				if (emitterList[i].particles[j].live)
				{
					setUniform("Model", UNIFORM::MAT4, glm::value_ptr(emitterList[i].particles[j].trans.get()));
					setUniform("diffuseTint", UNIFORM::FLO3, glm::value_ptr(emitterList[i].particles[j].color));					
					glDrawElements(GL_TRIANGLES, AssetManager::instance().get<ASSET::SIZE>("Quad"), GL_UNSIGNED_INT, 0);
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
