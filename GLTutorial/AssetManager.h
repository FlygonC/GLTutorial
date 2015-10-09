#pragma once
#include <unordered_map>
#include <string>

namespace AssetLibrary
{
	namespace ASSET
	{
		enum GL_Handel_Type { NONE = 0, VAO, VBO, IBO, SIZE, FBO, RBO, TEXTURE, SHADER, eSIZE };
	}

	typedef std::pair<ASSET::GL_Handel_Type, std::string> AssetKey;
	typedef unsigned int GL_Handel;

	class AssetManager
	{
		struct Hash
		{
			size_t operator()(AssetKey k) const
			{
				return std::hash<std::string>()(k.second.append(std::to_string((unsigned int)k.first)));
			}
		};

		//Map to hold Handels
		std::unordered_map<AssetKey, GL_Handel, Hash> handels;

		AssetManager() {  }

		GL_Handel getVerified(const AssetKey &key) const;

		bool setInternal(ASSET::GL_Handel_Type type, char* name, GL_Handel);

	public:
		static AssetManager &instance()
		{
			static AssetManager a;
			return a;
		}
		//Get Asset
		GL_Handel get(ASSET::GL_Handel_Type type, const char* name) const
		{
			return getVerified(AssetKey(type, name));
		}
		//Template get Asset
		template<ASSET::GL_Handel_Type t>
		GL_Handel get(const char* name) const
		{
			return getVerified(AssetKey(t, name));
		}
		//Asset reference get
		GL_Handel get(const AssetKey &key) const
		{
			return getVerified(key);
		}
		//fetch with asset object
		GL_Handel operator[](const AssetKey &key) const
		{
			return getVerified(key);
		}

		const void* getUniform(const AssetKey &key)
		{
			return handels.find(key)._Ptr;
		}


		bool buildVAO		(const char* name, const struct Vertex *verts, unsigned int vsize, const unsigned int *indices, unsigned int isize);
		bool buildFBO		(const char* name, unsigned int w, unsigned int h, unsigned int nTextures, const char* names[], const unsigned int depths[]);
		bool buildTexture	(const char* name, unsigned int w, unsigned int h, unsigned int depth, const char* pixels = nullptr);
		bool loadTexture	(const char* name, const char* path);
		bool loadShader		(const char* name, const char* vertexpath, const char* fragmentpath);
		bool loadFBX		(const char* name, const char* path);
		bool loadOBJ		(const char* name, const char* path);

		bool init();
		void kill();
	};
}