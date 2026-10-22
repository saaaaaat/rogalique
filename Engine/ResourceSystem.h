#pragma once

#include <map>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

namespace Engine
{
	class ENGINE_API ResourceSystem
	{
	public:
		static ResourceSystem* Instance();

		void LoadTexture(const std::string& name, std::string sourcePath, bool isSmooth = true);
		const sf::Texture* GetTextureShared(const std::string& name) const;
		sf::Texture* GetTextureCopy(const std::string& name) const;
		void DeleteSharedTexture(const std::string& name);

		void LoadTextureMap(const std::string& name, std::string sourcePath, sf::Vector2u elementPixelSize, int totalElements, bool isSmooth = true);
		const sf::Texture* GetTextureMapElementShared(const std::string& name, int elementIndex) const;
		sf::Texture* GetTextureMapElementCopy(const std::string& name, int elementIndex) const;
		int GetTextureMapElementsCount(const std::string& name) const;
		void DeleteSharedTextureMap(const std::string& name);

		void LoadMusic(const std::string& name, const std::string& sourcePath);
		sf::Music* GetMusic(const std::string& name);
		

		void Clear();

	private:
		std::map<std::string, sf::Texture*> textures;
		std::map<std::string, std::vector<sf::Texture*>> textureMaps;

		// хранилище музыки
		std::map<std::string, sf::Music*> musics;

		ResourceSystem() {}
		~ResourceSystem() {}

		ResourceSystem(ResourceSystem const&) = delete;
		ResourceSystem& operator= (ResourceSystem const&) = delete;

		void DeleteAllTextures();
		void DeleteAllTextureMaps();
	};
}