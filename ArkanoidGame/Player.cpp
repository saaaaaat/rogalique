#include "Player.h"
#include <ResourceSystem.h>

namespace RogaliqueGame
{
	Player::Player()
	{
		gameObject = Engine::GameWorld::Instance()->CreateGameObject();
		auto playerRenderer = gameObject->AddComponent<Engine::SpriteRendererComponent>();

		playerRenderer->SetTexture(*Engine::ResourceSystem::Instance()->GetTextureShared("ball"));
		playerRenderer->SetPixelSize(32, 32);

		auto playerCamera = gameObject->AddComponent<Engine::CameraComponent>();
		playerCamera->SetWindow(&Engine::RenderSystem::Instance()->GetMainWindow());
		playerCamera->SetBaseResolution(1280, 720);

		auto playerInput = gameObject->AddComponent<Engine::InputComponent>();
	}

	Engine::GameObject* Player::GetGameObject()
	{
		return gameObject;
	}
}