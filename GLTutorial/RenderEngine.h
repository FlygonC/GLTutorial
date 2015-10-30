#pragma once

#include <vector>

#include "Render.h"
#include "AssetManager.h"
#include "Camera.h"

using namespace AssetLibrary;

namespace RenderEngine
{
	struct Transform
	{
		glm::vec3 position = glm::vec3(0);
		glm::vec3 scale = glm::vec3(1);
		glm::vec3 rotation = glm::vec3(0);
		glm::mat4 get()
		{
			glm::mat4 r = glm::mat4(1);
			r *= glm::scale(scale) * 
				( glm::rotate(rotation.x, glm::vec3(1,0,0)) * glm::rotate(rotation.y, glm::vec3(0, 1, 0)) * glm::rotate(rotation.z, glm::vec3(0, 0, 1)) ) *
				glm::translate(position);
			return r;
		}
	};
	struct Material
	{
		glm::vec3 diffuseTint = glm::vec3(0);
		glm::vec3 specularTint = glm::vec3(0);
		float specularPower = 0;

		Asset<ASSET::TEXTURE> diffuseTexture;
		Asset<ASSET::TEXTURE> normalTexture;
		Asset<ASSET::TEXTURE> specularTexture;

		//Material() : diffuseTexture("White"), normalTexture("White"), specularTexture("White") {}
		Material operator=(Material other)
		{
			this->diffuseTint = other.diffuseTint;
			this->specularTint = other.specularTint;
			this->diffuseTint = other.diffuseTint;

			this->diffuseTexture = other.diffuseTexture;
			this->normalTexture = other.normalTexture;
			this->specularTexture = other.specularTexture;

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
		glm::vec3 color = glm::vec3(1);
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
		glm::vec3 color = glm::vec3(1);
		glm::vec3 position = glm::vec3(0);
		float radius = 1;
		bool visible = true;

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

		class GPassRender : public RenderPass
		{
		public:
			void prep();
			void post();
			void draw(RenderObjectIn ob, Camera c);
		};
		class DLightPassRender : public RenderPass
		{
		public:
			void prep();
			void post();
			void draw(DirectionalLightIn li, Camera c);
		};
		class PLightPassRender : public RenderPass
		{
		public:
			void prep();
			void post();
			void draw(PointLightIn li, Camera c);
		};
		class CompositePassRender : public RenderPass
		{
		public:
			void prep();
			void post();
			void draw();
		};

		std::vector<RenderObjectIn> objectList = std::vector<RenderObjectIn>();
		std::vector<DirectionalLightIn> directionalLightList = std::vector<DirectionalLightIn>();
		std::vector<PointLightIn> pointLightList = std::vector<PointLightIn>();

		GPassRender gPass;
		DLightPassRender dLight;
		PLightPassRender pLight;
		CompositePassRender comp;

		Camera camera;

		Renderer() {}

		unsigned int newObject();
		unsigned int newDLight();
		unsigned int newPLight();

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
		
		void setCamera(Camera c);

		void render();
	};
}