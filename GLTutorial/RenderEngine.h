#pragma once

#include <vector>
#include <unordered_map>

#include <glm\glm.hpp>

#include "AssetLibrary.h"

using namespace AssetLibrary;

namespace RenderEngine
{
	struct Material
	{
		glm::vec3 diffuseTint = glm::vec3(1);
		glm::vec3 specularTint = glm::vec3(1);
		float specularPower = 0;

		Asset<ASSET::TEXTURE> diffuseTexture;
		Asset<ASSET::TEXTURE> normalTexture;
		Asset<ASSET::TEXTURE> specularTexture;

		Material() {}
	};

	class GameObject
	{
		Asset<ASSET::VAO> mesh;
		Material mat;

	public:
		bool visible = true;
		glm::vec3 position = glm::vec3(0);

		void setMesh(Asset<ASSET::VAO> m);
		void setMaterial(Material m);
	};

	class RenderEngine
	{
		
		//std::vector<GameObject> objectList = std::vector<GameObject>();
		std::unordered_map<unsigned int, GameObject> objectMap = std::unordered_map<unsigned int, GameObject>();

		RenderEngine() {}

	public:
		RenderEngine instance()
		{
			static RenderEngine a;
			return a;
		}

		unsigned int createGameObject();
		GameObject getGameObject(const char* name);
	};
}