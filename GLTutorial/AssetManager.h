#pragma once
#include <unordered_map>
#include <string>

#include <gl_core_4_4.h>
#include <stb_image.h>

#include "ShaderLoader.h"

namespace AssetLibrary
{
	namespace ASSET
	{
		enum GL_Handle_Type { NONE = 0, VAO, VBO, IBO, SIZE, FBO, RBO, TEXTURE, SHADER, eSIZE };
	}

	typedef std::pair<ASSET::GL_Handle_Type, std::string> AssetKey;
	typedef unsigned int GL_Handle;

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
		std::unordered_map<AssetKey, GL_Handle, Hash> handles;

		AssetManager() {  }

		GL_Handle getVerified(const AssetKey &key) const;

		bool setInternal(ASSET::GL_Handle_Type type, const char* name, GL_Handle);

	public:
		static AssetManager &instance()
		{
			static AssetManager a;
			return a;
		}
		//Get Asset
		GL_Handle get(ASSET::GL_Handle_Type type, const char* name) const
		{
			return getVerified(AssetKey(type, name));
		}
		//Get Asset: Fancy Templated Edition
		template<ASSET::GL_Handle_Type t>
		GL_Handle get(const char* name) const
		{
			return getVerified(AssetKey(t, name));
		}
		//Asset reference get
		GL_Handle get(const AssetKey &key) const
		{
			return getVerified(key);
		}
		//fetch with asset object
		GL_Handle operator[](const AssetKey &key) const
		{
			return getVerified(key);
		}

		const void* getUniform(const AssetKey &key)
		{
			return handles.find(key)._Ptr;
		}


		bool buildVAO		(const char* name, const struct Vertex *verts, unsigned int vsize, const unsigned int *indices, unsigned int isize);
		bool buildFBO		(const char* name, unsigned int w, unsigned int h, unsigned int nTextures, const char* names[], const unsigned int depths[], bool hasDepth);
		bool buildTexture	(const char* name, unsigned int w, unsigned int h, unsigned int depth, const char* pixels = nullptr);
		bool loadTexture	(const char* name, const char* path);
		bool loadShader		(const char* name, const char* vertexpath, const char* fragmentpath);
		bool loadFBX		(const char* name, const char* path);
		bool loadOBJ		(const char* name, const char* path);

		bool init();
		void kill();
	};
}