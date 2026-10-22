#include "pch.h"
#include "ResourceSystem.h"
#include <iostream>

namespace Engine
{
	ResourceSystem* ResourceSystem::Instance()
	{
		static ResourceSystem resourceSystem;
		return &resourceSystem;
	}


	void ResourceSystem::LoadTexture(const std::string& name, std::string sourcePath, bool isSmooth) {
		if (textures.find(name) != textures.end()) return;
		sf::Texture* newTexture = new sf::Texture();
		if (newTexture->loadFromFile(sourcePath)) {
			newTexture->setSmooth(isSmooth);
			textures.emplace(name, newTexture);
		}
	}
	const sf::Texture* ResourceSystem::GetTextureShared(const std::string& name) const { return textures.find(name)->second; }
	sf::Texture* ResourceSystem::GetTextureCopy(const std::string& name) const { return new sf::Texture(*textures.find(name)->second); }
	void ResourceSystem::DeleteSharedTexture(const std::string& name) { }
	void ResourceSystem::LoadTextureMap(const std::string& name, std::string sourcePath, sf::Vector2u elementPixelSize, int totalElements, bool isSmooth) {  }
	const sf::Texture* ResourceSystem::GetTextureMapElementShared(const std::string& name, int elementIndex) const { return nullptr; }
	sf::Texture* ResourceSystem::GetTextureMapElementCopy(const std::string& name, int elementIndex) const { return nullptr;  }
	int ResourceSystem::GetTextureMapElementsCount(const std::string& name) const { return 0;  }
	void ResourceSystem::DeleteSharedTextureMap(const std::string& name) {  }
	void ResourceSystem::Clear() { DeleteAllTextures(); DeleteAllTextureMaps(); }
	void ResourceSystem::DeleteAllTextures() {  }
	void ResourceSystem::DeleteAllTextureMaps() {  }

	
	void ResourceSystem::LoadMusic(const std::string& name, const std::string& sourcePath)
	{
		if (musics.find(name) != musics.end()) return;

		sf::Music* newMusic = new sf::Music();
		if (newMusic->openFromFile(sourcePath)) {
			musics.emplace(name, newMusic);
		}
		else {
			std::cout << "Ошибка: не удалось загрузить музыку: " << sourcePath << std::endl;
		}
	}

	sf::Music* ResourceSystem::GetMusic(const std::string& name)
	{
		if (musics.find(name) != musics.end()) {
			return musics[name];
		}
		return nullptr;
	}
}