#include "RenderEngine.h"

using namespace RenderEngine;
// Geometry
GeometryObject::GeometryObject()
{
	idNum = Renderer::instance().newObject(DATATYPE::GEOMETRY);
	data = &Renderer::instance().geometryData[idNum];
}
GeometryObject::~GeometryObject()
{
	Renderer::instance().clearObject(DATATYPE::GEOMETRY, idNum);
}
// Directional Light
RenderEngine::DirectionalLightObject::DirectionalLightObject()
{
	idNum = Renderer::instance().newObject(DATATYPE::DIRECTIONALLIGHT);
	data = &Renderer::instance().directionalLightData[idNum];
}
RenderEngine::DirectionalLightObject::~DirectionalLightObject()
{
	Renderer::instance().clearObject(DATATYPE::DIRECTIONALLIGHT, idNum);
}
// Point Light
RenderEngine::PointLightObject::PointLightObject()
{
	idNum = Renderer::instance().newObject(DATATYPE::POINTLIGHT);
	data = &Renderer::instance().pointLightData[idNum];
}
RenderEngine::PointLightObject::~PointLightObject()
{
	Renderer::instance().clearObject(DATATYPE::POINTLIGHT, idNum);
}
// Particle Emitter
void RenderEngine::ParticleEmitter::createBuffers()
{
	maxParts = 10000;
	particles = new Particle[maxParts];
	for (unsigned i = 0; i < 2; i++)
	{
		glGenVertexArrays(1, &VAO[i]);
		glGenBuffers(1, &VBO[i]);
		//glGenBuffers(1, &iboHandle);

		//Set Data
		glBindVertexArray(VAO[i]);
		//vertex
		glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Particle) * maxParts, particles, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		//glEnableVertexAttribArray(4);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), 0);//vec3
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(sizeof(float) * 3));//vec3
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(sizeof(float) * 6));//float
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(sizeof(float) * 7));//float
		//glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(sizeof(float) * 8));//float
		//unbind
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}
void RenderEngine::ParticleEmitter::clearBuffers()
{
	for (unsigned i = 0; i < 2; i++)
	{
		glDeleteBuffers(1, &VAO[i]);
		glDeleteBuffers(1, &VBO[i]);
	}
}
RenderEngine::ParticleEmitterObject::ParticleEmitterObject()
{
	idNum = Renderer::instance().newObject(DATATYPE::PARTICLEEMITTER);
	data = &Renderer::instance().emitterData[idNum];
}

RenderEngine::ParticleEmitterObject::~ParticleEmitterObject()
{
	Renderer::instance().clearObject(DATATYPE::PARTICLEEMITTER, idNum);
	data->clearBuffers();
}

// Object Managment
unsigned int Renderer::newObject(DATATYPE::TYPE type)
{
	switch (type)
	{
	case DATATYPE::GEOMETRY:
		for (unsigned i = 0; i < objectCount; i++)
		{
			if (geometryData[i].inUse == false)
			{
				geometryData[i].inUse = true;
				return i;
			}
		}
		break;

	case DATATYPE::DIRECTIONALLIGHT:
		for (unsigned i = 0; i < objectCount; i++)
		{
			if (directionalLightData[i].inUse == false)
			{
				directionalLightData[i].inUse = true;
				return i;
			}
		}
		break;

	case DATATYPE::POINTLIGHT:
		for (unsigned i = 0; i < objectCount; i++)
		{
			if (pointLightData[i].inUse == false)
			{
				pointLightData[i].inUse = true;
				return i;
			}
		}
		break;

	case DATATYPE::PARTICLEEMITTER:
		for (unsigned i = 0; i < objectCount; i++)
		{
			if (emitterData[i].inUse == false)
			{
				emitterData[i].inUse = true;
				//emitterData[i].createBuffers();
				return i;
			}
		}
		break;

	default:
		break;
	}
	return -1;
}
void Renderer::clearObject(DATATYPE::TYPE type, unsigned int tag)
{
	switch (type)
	{
	case DATATYPE::GEOMETRY:
		geometryData[tag].inUse = false;
		break;

	case DATATYPE::DIRECTIONALLIGHT:
		directionalLightData[tag].inUse = false;
		break;

	case DATATYPE::POINTLIGHT:
		pointLightData[tag].inUse = false;
		break;

	case DATATYPE::PARTICLEEMITTER:
		emitterData[tag].inUse = false;
		break;

	default:
		break;
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
	for (unsigned i = 0; i < objectCount; i++)
	{
		emitterData[i].createBuffers();
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

	//PARTICLES
	a.loadShaderG("ParticleDrawShader", "./DeferredRendering/Particles/ParticleDrawVertex.txt", "./DeferredRendering/Particles/ParticleDrawGeometry.txt", "./DeferredRendering/Particles/ParticleDrawFragment.txt");
	a.buildParticleUpdateShader("ParticleUpdateShader", "./DeferredRendering/Particles/ParticleUpdateVertex.txt");

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
	// Geometry Pass ########################
	// Init
	glBindFramebuffer(GL_FRAMEBUFFER, AssetManager::instance().get<ASSET::FBO>("GPassFrameBuffer"));
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	bindShader(AssetManager::instance().get<ASSET::SHADER>("GPassShader"));
	// Draw
	setUniform("ProjectionView", UNIFORM::MAT4, glm::value_ptr(camera.getProjectionView()));
	setUniform("View", UNIFORM::MAT4, glm::value_ptr(camera.getView()));

	setUniform("ambientLight", UNIFORM::FLO3, glm::value_ptr(ambientLight));

	for (unsigned i = 0; i < objectCount; i++)
	{
		//GeometryData iter = geometryData[i];
		if (geometryData[i].inUse && geometryData[i].visible)
		{
			setUniform("Model",			UNIFORM::MAT4, glm::value_ptr(geometryData[i].transform.get()));

			setUniform("specPower",		UNIFORM::FLO1, &geometryData[i].material.specularPower);

			setUniform("diffuseTint",	UNIFORM::FLO3, glm::value_ptr(geometryData[i].material.diffuseTint));
			setUniform("specularTint",	UNIFORM::FLO3, glm::value_ptr(geometryData[i].material.specularTint));
			setUniform("glowTint",		UNIFORM::FLO3, glm::value_ptr(geometryData[i].material.glowTint));

			setUniform("diffuseMap",	UNIFORM::TEX2, geometryData[i].material.diffuseTexture, 0);
			setUniform("normalMap",		UNIFORM::TEX2, geometryData[i].material.normalTexture, 1);
			setUniform("specularMap",	UNIFORM::TEX2, geometryData[i].material.specularTexture, 2);
			setUniform("glowMap",		UNIFORM::TEX2, geometryData[i].material.glowTexture, 3);

			glBindVertexArray(AssetManager::instance().get<ASSET::VAO>(geometryData[i].mesh.name.c_str()));
			glDrawElements(GL_TRIANGLES, AssetManager::instance().get<ASSET::SIZE>(geometryData[i].mesh.name.c_str()), GL_UNSIGNED_INT, 0);
		}
	}

	// PARTICLES
	/////////////////////////////////////////////////////////////////////////////
	// (UPDATE SHADER)
	// use program
	// bind uniforms etc.
	// disable rasterization
	// bind VAO and VBO for base buffer, use beginTRansformFeedback
	// do the draw and disable transform feedback

	// (Draw Shader)
	// works the same as usual.

	/////////////////////////////////////////////////////////////////////////////
	for (unsigned i = 0; i < objectCount; i++)
	{
		if (emitterData[i].inUse)
		{
			//Update
			bindShader(AssetManager::instance().get<ASSET::SHADER>("ParticleUpdateShader"));

			float time = Window::instance().getTime();
			setUniform("time", UNIFORM::FLO1, &time);
			setUniform("deltaTime", UNIFORM::FLO1, &deltaTime);
			setUniform("lifeMin", UNIFORM::FLO1, &emitterData[i].maxLife.start);
			setUniform("lifeMax", UNIFORM::FLO1, &emitterData[i].maxLife.end);
			setUniform("emitterPosition", UNIFORM::FLO3, &emitterData[i].transform.position);

			glEnable(GL_RASTERIZER_DISCARD);
			// Read from ActiveBuffer
			glBindVertexArray(emitterData[i].VAO[emitterData[i].activeBuffer]);
			unsigned int otherBuffer = (emitterData[i].activeBuffer + 1) % 2;
			// Write to OtherBuffer
			glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, emitterData[i].VBO[otherBuffer]);
			glBeginTransformFeedback(GL_POINTS);

			glDrawArrays(GL_POINTS, 0, emitterData[i].getMaxParts());

			glEndTransformFeedback();
			glDisable(GL_RASTERIZER_DISCARD);
			glBindBufferBase(GL_TRANSFORM_FEEDBACK, 0, 0);
			//Draw
			bindShader(AssetManager::instance().get<ASSET::SHADER>("ParticleDrawShader"));

			setUniform("ProjectionView", UNIFORM::MAT4, glm::value_ptr(camera.getProjectionView()));
			setUniform("WorldTransform", UNIFORM::MAT4, glm::value_ptr(camera.getWorldTransform()));

			setUniform("sizeStart", UNIFORM::FLO1, &emitterData[i].size.start);
			setUniform("sizeEnd",	UNIFORM::FLO1, &emitterData[i].size.end);
			setUniform("colorStart", UNIFORM::FLO4, glm::value_ptr(glm::vec4(emitterData[i].color.start, 0)));
			setUniform("colorEnd",	 UNIFORM::FLO4, glm::value_ptr(glm::vec4(emitterData[i].color.end, 0)));

			glBindVertexArray(emitterData[i].VAO[otherBuffer]);
			glDrawArrays(GL_POINTS, 0, emitterData[i].getMaxParts());
			//glDrawArrays(GL_POINTS, 0, 1);

			emitterData[i].activeBuffer = otherBuffer;
		}
	}
	// GPass Post
	glDisable(GL_DEPTH_TEST);

	//PRE LIGHT CLEAR
	glBindFramebuffer(GL_FRAMEBUFFER, AssetManager::instance().get<ASSET::FBO>("LightFrameBuffer"));
	glClear(GL_COLOR_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Directional Light & Shadow ########################
	for (unsigned i = 0; i < objectCount; i++)
	{
		if (directionalLightData[i].inUse && directionalLightData[i].visible)
		{
			// Shadow %%%%%%%%
/*prep*/	glm::vec3 m_lightDirection = glm::normalize(directionalLightData[i].direction);
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

/*draw*/	for (unsigned j = 0; j < objectCount; j++)
			{
				if (geometryData[j].inUse && geometryData[j].visible)
				{
					setUniform("Model", UNIFORM::MAT4, glm::value_ptr(geometryData[j].transform.get()));

					glBindVertexArray(AssetManager::instance().get<ASSET::VAO>(geometryData[j].mesh.name.c_str()));
					glDrawElements(GL_TRIANGLES, AssetManager::instance().get<ASSET::SIZE>(geometryData[j].mesh.name.c_str()), GL_UNSIGNED_INT, 0);
				}
			}

/*post*/	glViewport(0, 0, Window::instance().getWidth(), Window::instance().getHeight());
			glDisable(GL_DEPTH_TEST);
			glFrontFace(GL_CCW);
			glDisable(GL_CULL_FACE);


			// Directional Light %%%%%%%%
			// DLight prep
			glBindFramebuffer(GL_FRAMEBUFFER, AssetManager::instance().get<ASSET::FBO>("LightFrameBuffer"));
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);

			bindShader(AssetManager::instance().get<ASSET::SHADER>("DLightShader"));
			// Draw
			setUniform("cameraView", UNIFORM::MAT4, glm::value_ptr(camera.getView()));

			glm::vec3 d = glm::vec3(camera.getWorldTransform()[3]);
			//std::cout << d.x << " " << d.y << " " << d.z << std::endl;
			setUniform("cameraPosition", UNIFORM::FLO3, glm::value_ptr(d));
		
			setUniform("lightDirection", UNIFORM::FLO3, glm::value_ptr(directionalLightData[i].direction));
			setUniform("lightDiffuse", UNIFORM::FLO3, glm::value_ptr(directionalLightData[i].color));

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
	// DLight Post
	glDisable(GL_BLEND);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);


	// Point Light ########################
	// PLight Prep
	glBindFramebuffer(GL_FRAMEBUFFER, AssetManager::instance().get<ASSET::FBO>("LightFrameBuffer"));
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_FRONT);
	
	bindShader(AssetManager::instance().get<ASSET::SHADER>("PLightShader"));
	// Draw
	for (unsigned i = 0; i < objectCount; i++)
	{
		if (pointLightData[i].inUse && pointLightData[i].visible)
		{
			setUniform("ProjectionView", UNIFORM::MAT4, glm::value_ptr(camera.getProjectionView()));
			setUniform("cameraView", UNIFORM::MAT4, glm::value_ptr(camera.getView()));
			setUniform("cameraPosition", UNIFORM::FLO3, glm::value_ptr(camera.getWorldTransform()[3]));

			setUniform("lightPositionView", UNIFORM::FLO3, glm::value_ptr(camera.getView() * glm::vec4(pointLightData[i].position, 1)));

			setUniform("lightPosition", UNIFORM::FLO3, glm::value_ptr(pointLightData[i].position));
			setUniform("lightDiffuse", UNIFORM::FLO3, glm::value_ptr(pointLightData[i].color));
			setUniform("lightRadius", UNIFORM::FLO1, &pointLightData[i].radius);

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
	// PLight Post
	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);


	// Composite Pass ########################
	// Comp Prep
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	bindShader(AssetManager::instance().get<ASSET::SHADER>("CompositeShader"));
	// Draw
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

	// Comp Post
	glUseProgram(0);
}