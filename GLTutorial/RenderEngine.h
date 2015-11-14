#pragma once

#include <vector>

#include "Render.h"
#include "AssetManager.h"
#include "Camera.h"

using namespace AssetLibrary;

namespace RenderEngine
{
	namespace UNIFORM
	{
		enum TYPE { NONE = 0, FLO1, FLO2, FLO3, FLO4, MAT4, INT1, TEX2, eSIZE };
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

	struct RenderObjectEx
	{
	private:
		unsigned int referenceID;
	public:
		Asset<ASSET::VAO> mesh;
		Transform transform;
		Material material;
		bool visible = true;

		void instantiate();
		void update();
		void destroy();
		unsigned int getReferenceID();
	};
	struct DirectionalLightEx
	{
	private:
		unsigned int referenceID;
	public:
		glm::vec3 color = glm::vec3(0);
		glm::vec3 direction = glm::vec3(1);
		bool visible = true;

		void instantiate();
		void update();
		void destroy();
		unsigned int getReferenceID();
	};
	struct PointLightEx
	{
	private:
		unsigned int referenceID;
	public:
		glm::vec3 color = glm::vec3(0);
		glm::vec3 position = glm::vec3(0);
		float radius = 1;
		bool visible = true;

		void instantiate();
		void update();
		void destroy();
		unsigned int getReferenceID();
	};
	

	template<typename T>
	struct Range
	{
		T start, end;
		operator=(T other)
		{
			start = other;
			end = other;
		}
	};
	struct Particle
	{
		Transform trans;
		Material mat;
		glm::vec3 direction = glm::vec3(0);
		float velocity;
		float lifeTime = 0;
	};
	class ParticleEmitterEx
	{
		unsigned int referenceID;
		//Particle particles[50];
	public:
		Range<Transform> transform;
		Range<float> velocity;
		Range<glm::vec3> direction;
		Range<glm::vec3> color;
		float maxLife;
		float emittionRate;

		Asset<ASSET::VAO> mesh;

		void instantiate();
		void update();
		void destroy();
		unsigned int getReferenceID();
	};
	
	class Renderer
	{
		struct RenderObjectIn
		{
			Asset<ASSET::VAO> mesh;
			Transform transform;
			Material material;
			bool visible = true;

			bool inUse = false;

			RenderObjectIn operator=(RenderObjectEx other)
			{
				this->mesh = other.mesh;
				this->transform = other.transform;
				this->material = other.material;
				this->visible = other.visible;
				return *this;
			}
		};
		struct DirectionalLightIn
		{
			glm::vec3 color = glm::vec3(1);
			glm::vec3 direction = glm::vec3(1);
			bool visible = true;

			bool inUse = false;

			DirectionalLightIn operator=(DirectionalLightEx other)
			{
				this->color = other.color;
				this->direction = other.direction;
				return *this;
			}
		};
		struct PointLightIn
		{
			glm::vec3 color = glm::vec3(1);
			glm::vec3 position = glm::vec3(0);
			float radius = 1;
			bool visible = true;

			bool inUse = false;

			PointLightIn operator=(PointLightEx other)
			{
				this->color = other.color;
				this->position = other.position;
				this->radius = other.radius;
				return *this;
			}
		};

		class ParticleEmitterIn
		{
		public:
			Particle particles[50];

			Range<Transform> transform;
			Range<float> velocity;
			Range<glm::vec3> direction;
			Range<glm::vec3> color;
			float maxLife;
			float emittionRate;

			Asset<ASSET::VAO> mesh;

			bool inUse = false;

			void createPart()
			{
				particles[currentPart].trans = transform.start;
				particles[currentPart].velocity = velocity.start;
				particles[currentPart].direction = glm::lerp(direction.start, direction.end, 1.f);
			}
			void update(float deltaTime);

			ParticleEmitterIn operator=(ParticleEmitterEx other)
			{
				this->transform = other.transform;
				this->velocity = other.velocity;
				this->direction = other.direction;
				this->color = other.color;
				this->maxLife = other.maxLife;
				this->emittionRate = other.emittionRate;

				this->mesh = other.mesh;

				return *this;
			}
		private:
			int currentPart = 0;
		};

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

		std::vector<RenderObjectIn> objectList = std::vector<RenderObjectIn>();
		std::vector<DirectionalLightIn> directionalLightList = std::vector<DirectionalLightIn>();
		std::vector<PointLightIn> pointLightList = std::vector<PointLightIn>();
		std::vector<ParticleEmitterIn> emitterList = std::vector<ParticleEmitterIn>();

		GPassRender gPass;
		DLightPassRender dLight;
		PLightPassRender pLight;
		CompositePassRender comp;

		unsigned int boundShader;
		unsigned int boundFrameBuffer;

		glm::vec3 ambientLight = glm::vec3(0.5f);
		Camera camera;

		Renderer() {}

		unsigned int newObject();
		unsigned int newDLight();
		unsigned int newPLight();
		unsigned int newEmitter();

		//void bindFBO(unsigned int fbo);
		void bindShader(unsigned int shader);
		bool setUniform(const char* name, UNIFORM::TYPE type, const void* value, unsigned count = 1, bool normalize = false);

	public:
		static Renderer &instance()
		{
			static Renderer a;
			return a;
		}

		void init();
		void kill();

		unsigned int createObject(RenderObjectEx* in);
		void updateObject(RenderObjectEx* in, unsigned int id);
		void clearObject(unsigned int id);
		
		unsigned int createDLight(DirectionalLightEx* in);
		void updateDLight(DirectionalLightEx* in, unsigned int id);
		void clearDLight(unsigned int id);

		unsigned int createPLight(PointLightEx* in);
		void updatePLight(PointLightEx* in, unsigned int id);
		void clearPLight(unsigned int id);

		unsigned int createEmitter(ParticleEmitterEx* in);
		void updateEmitter(ParticleEmitterEx* in, unsigned int id);
		void clearEmitter(unsigned int id);
		
		void setCamera(Camera c);

		void render();
	};
}