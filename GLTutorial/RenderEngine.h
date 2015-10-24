#pragma once

#include <vector>

#include <glm\glm.hpp>

#include "AssetLibrary.h"
#include "GPass.h"
#include "Window.h"

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
			r *= glm::scale(scale) * glm::rotate(1.f, rotation) * glm::translate(position);
		}
	};

	struct Material
	{
		glm::vec3 diffuseTint = glm::vec3(1);
		glm::vec3 specularTint = glm::vec3(1);
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

			this->diffuseTexture.name = other.diffuseTexture.name;
			this->normalTexture.name = other.normalTexture.name;
			this->specularTexture.name = other.specularTexture.name;

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

	class Renderer
	{
		struct RenderObjectIn
		{
			//RenderObjectEx* externalReference = new RenderObjectEx();
			Asset<ASSET::VAO> mesh;
			Transform transform;
			Material material;
			bool visible = true;

			bool inUse = false;

			RenderObjectIn operator=(RenderObjectEx other)
			{
				this->mesh.name = other.mesh.name;
				this->transform = other.transform;
				this->material = other.material;
				this->visible = other.visible;
				return *this;
			}
		};

		std::vector<RenderObjectIn> objectList = std::vector<RenderObjectIn>();

		//unsigned int nextEmpty = 0;

		GPass gPass;
		LightPass lightPass;
		CompositePass compositePass;

		Renderer() {}

		unsigned int newObject();

	public:
		static Renderer &instance()
		{
			static Renderer a;
			return a;
		}

		void init();
		unsigned int createObject(RenderObjectEx* in);
		void updateObject(RenderObjectEx* in, unsigned int id);
		void clearObject(unsigned int id);
		//RenderObject getObject(unsigned int index);
		void render(Window w);
	};
}