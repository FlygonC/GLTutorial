#pragma once

#include <vector>
#include <algorithm>

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
	namespace DATATYPE
	{
		enum TYPE { GEOMETRY = 0, DIRECTIONALLIGHT, POINTLIGHT, PARTICLEEMITTER, EMPTY = 100 };
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
		typename type = T;
		T start, end;
		void operator=(T other)
		{
			start = other;
			end = other;
		}
		void func()
		{
			if (type == float)
			{

			}
		}
	};

	class DataContainer
	{
	public:
		glm::vec3 position = glm::vec3(0);
		bool visible = true;
	};
	class GeometryData : public DataContainer
	{
	public:
		Asset<ASSET::VAO> mesh;
		Transform transform;
		Material material;
	};
	class DirectionalLightData : public DataContainer
	{
	public:
		glm::vec3 color = glm::vec3(0);
		glm::vec3 direction = glm::vec3(1);
	};
	class PointLightData : public DataContainer
	{
	public:
		glm::vec3 color = glm::vec3(0);
		glm::vec3 position = glm::vec3(0);
		float radius = 1;
	};
	
	struct Particle
	{
		float position;
		float velocity;
		float size;
		float lifeTime, maxLife;
		/*Transform trans;
		Material mat;
		glm::vec3 direction = glm::vec3(0);
		float velocity;
		glm::vec3 color;
		float lifeTime = 0;
		bool live = false;*/
	};
	class ParticleEmitterData : public DataContainer
	{
		Range<float> velocity;
		Range<glm::vec3> direction;
		Range<glm::vec3> size;
		Range<float> maxLife;
		Range<float> emittionRate;
	};
	


	//template<DataContainer T>
	class RenderObjectEx
	{
		DATATYPE::TYPE dataType;
		unsigned int tagNumber;

		RenderObjectEx() {}
		~RenderObjectEx();
	public :
		DataContainer data;

		RenderObjectEx(DATATYPE::TYPE type);
		void update();
		void destroy(unsigned int tag);
	};
	
	class Renderer
	{
/*		struct RenderObjectIn
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

		};
*/

		//template<DataContainer T>
		class RenderObjectIn
		{
		public:
			RenderObjectIn() {}

			DATATYPE::TYPE dataType = DATATYPE::EMPTY;
			unsigned int tagNumber;
			bool inUse = false;

			//DataContainer data;

			void clear()
			{
				dataType = DATATYPE::EMPTY;
				tagNumber = NULL;
				inUse = false;
				//delete data;
				//data = nullptr;
			}
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

		unsigned int typeCounts[4];
		std::vector<RenderObjectIn> objectList = std::vector<RenderObjectIn>();
		//std::vector<RenderObjectIn> objectList = std::vector<RenderObjectIn>();
		//std::vector<DirectionalLightIn> directionalLightList = std::vector<DirectionalLightIn>();
		//std::vector<PointLightIn> pointLightList = std::vector<PointLightIn>();
		//std::vector<ParticleEmitterIn> emitterList = std::vector<ParticleEmitterIn>();

		GPassRender gPass;
		DLightPassRender dLight;
		PLightPassRender pLight;
		CompositePassRender comp;

		unsigned int boundShader;
		unsigned int boundFrameBuffer;
		unsigned int objectCount = 0;

		glm::vec3 ambientLight = glm::vec3(0.5f);
		Camera camera;

		Renderer() {}

		//void bindFBO(unsigned int fbo);
		void bindShader(unsigned int shader);
		bool setUniform(const char* name, UNIFORM::TYPE type, const void* value, unsigned count = 1, bool normalize = false);

		static bool compare(const DATATYPE::TYPE &l, const DATATYPE::TYPE &r);

	public:
		static Renderer &instance()
		{
			static Renderer a;
			return a;
		}

		void init();
		void kill();

		unsigned int newObject(DATATYPE::TYPE type, DataContainer in);
		void updateObject(unsigned int tag, DataContainer in);
		void clearObject(unsigned int tag);
/*		unsigned int createObject(RenderObjectEx* in);
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
*/		
		void setCamera(Camera c);

		void render(float deltaTime);
	};
}