#pragma once

#include <vector>
#include <algorithm>

#include "Render.h"
#include "AssetManager.h"
#include "Camera.h"

#include "Particle.h"

using namespace AssetLibrary;

namespace RenderEngine
{
	namespace UNIFORM
	{
		enum TYPE { NONE = 0, FLO1, FLO2, FLO3, FLO4, MAT4, INT1, TEX2, eSIZE };
	}
	namespace DATATYPE
	{
		enum TYPE { UNDEFINED = -1, GEOMETRY = 0, DIRECTIONALLIGHT, POINTLIGHT, PARTICLEEMITTER };
	}

	struct Transform
	{
		glm::vec3 position = glm::vec3(0);
		glm::vec3 scale = glm::vec3(1);
		glm::vec3 rotation = glm::vec3(0);
		glm::mat4 get()
		{
			glm::mat4 r = glm::mat4(1);
			r *= glm::translate(position) *
				(glm::rotate(rotation.x, glm::vec3(1, 0, 0)) * glm::rotate(rotation.y, glm::vec3(0, 1, 0)) * glm::rotate(rotation.z, glm::vec3(0, 0, 1))) *
				glm::scale(scale);
			return r;
		}
	};
	struct Material
	{
		glm::vec3 diffuseTint = glm::vec3(0);
		glm::vec3 specularTint = glm::vec3(0);
		glm::vec3 glowTint = glm::vec3(0);
		float specularPower = 0;

		Asset<ASSET::TEXTURE> diffuseTexture;
		Asset<ASSET::TEXTURE> normalTexture;
		Asset<ASSET::TEXTURE> specularTexture;
		Asset<ASSET::TEXTURE> glowTexture;

		//Material() : diffuseTexture("White"), normalTexture("White"), specularTexture("White") {}
		Material operator=(Material other)
		{
			this->diffuseTint = other.diffuseTint;
			this->specularTint = other.specularTint;
			this->glowTint = other.glowTint;

			this->specularPower = other.specularPower;

			this->diffuseTexture = other.diffuseTexture;
			this->normalTexture = other.normalTexture;
			this->specularTexture = other.specularTexture;
			this->glowTexture = other.glowTexture;

			return *this;
		}
	};
	template<typename T>
	struct Range
	{
		//typename type = T;
		T start;
		T end;
		void operator=(T other)
		{
			start = other;
			end = other;
		}
	};



	class RenderObjectData
	{
	protected:
		//DATATYPE::TYPE type = DATATYPE::UNDEFINED;
	public:
		//glm::vec3 position = glm::vec3(0);
		bool visible = true;
		bool inUse = false;
	};

	class Geometry : public RenderObjectData
	{
	public:
		//GeometryData() { type = DATATYPE::GEOMETRY; }

		Asset<ASSET::VAO> mesh;
		Transform transform;
		Material material;
	};
	class DirectionalLight : public RenderObjectData
	{
	public:
		//DirectionalLightData() { type = DATATYPE::DIRECTIONALLIGHT; }

		glm::vec3 color = glm::vec3(1);
		glm::vec3 direction = glm::vec3(1);
	};
	class PointLight : public RenderObjectData
	{
	public:
		//PointLightData() { type = DATATYPE::POINTLIGHT; }

		glm::vec3 color = glm::vec3(1);
		glm::vec3 position = glm::vec3(0);
		float radius = 1;
	};	
	class ParticleEmitter : public RenderObjectData
	{
		unsigned int maxParts = 1000;
	public:
		//ParticleEmitterData() { type = DATATYPE::PARTICLEEMITTER; }
		void createBuffers();
		void clearBuffers();
		unsigned int getMaxParts()
		{
			return maxParts;
		}

		Transform transform;

		Particle *particles;

		Range<glm::vec3> velocity;
		//Range<glm::vec3> direction;
		Range<float> size;
		Range<glm::vec3> color;
		Range<float> maxLife;
		Range<float> emittionRate;

		unsigned int activeBuffer = 0;
		unsigned int VAO[2];
		unsigned int VBO[2];
	};
	


	class BaseObject 
	{
	public:
		BaseObject() {}
	};
	class GeometryObject : public BaseObject
	{
		unsigned int idNum;
	public:
		GeometryObject();
		~GeometryObject();

		Geometry* data;
	};
	class DirectionalLightObject : public BaseObject
	{
		unsigned int idNum;
	public:
		DirectionalLightObject();
		~DirectionalLightObject();

		DirectionalLight* data;
	};
	class PointLightObject : public BaseObject
	{
		unsigned int idNum;
	public:
		PointLightObject();
		~PointLightObject();

		PointLight* data;
	};
	class ParticleEmitterObject : public BaseObject
	{
		unsigned int idNum;
	public:
		ParticleEmitterObject();
		~ParticleEmitterObject();

		ParticleEmitter* data;
	};


	const unsigned int objectCount = 300;
	class Renderer
	{
		class GPassRender : public RenderPass
		{
		public:
			void prep();
			void post();
			//void draw(RenderObjectIn ob, Camera c);
		};
		class DLightPassRender : public RenderPass
		{
		public:
			void prep();
			void post();
			//void draw(DirectionalLightIn li, Camera c);
		};
		class PLightPassRender : public RenderPass
		{
		public:
			void prep();
			void post();
			//void draw(PointLightIn li, Camera c);
		};
		class CompositePassRender : public RenderPass
		{
		public:
			void prep();
			void post();
			//void draw();
		};

		GPassRender gPass;
		DLightPassRender dLight;
		PLightPassRender pLight;
		CompositePassRender comp;

		unsigned int boundShader;
		unsigned int boundFrameBuffer;

		glm::vec3 ambientLight = glm::vec3(0.1f);
		Camera camera;

		Renderer() {}

		//void bindFBO(unsigned int fbo);
		void bindShader(unsigned int shader);
		bool setUniform(const char* name, UNIFORM::TYPE type, const void* value, unsigned count = 1, bool normalize = false);

		//static bool compare(const DATATYPE::TYPE &l, const DATATYPE::TYPE &r);

	public:
		static Renderer &instance()
		{
			static Renderer a;
			return a;
		}

		void init();
		void kill();

		unsigned int newObject(DATATYPE::TYPE type);
		void updateObject(unsigned int tag, RenderObjectData* in);
		void clearObject(DATATYPE::TYPE type, unsigned int tag);


		void setCamera(Camera c);

		void render(float deltaTime);

		Geometry geometryData[objectCount];
		DirectionalLight directionalLightData[objectCount];
		PointLight pointLightData[objectCount];
		ParticleEmitter emitterData[objectCount];
	};
}