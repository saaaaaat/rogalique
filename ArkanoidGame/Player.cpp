#include "Player.h"
#include <ResourceSystem.h>

namespace RogaliqueGame
{
	Player::Player()
	{
		gameObject = Engine::GameWorld::Instance()->CreateGameObject("Player");
		if (gameObject == nullptr) return;

		
		startPosition = sf::Vector2f(640.f, 360.f);

		auto playerRenderer = gameObject->AddComponent<Engine::SpriteRendererComponent>();
		playerRenderer->SetTexture(*Engine::ResourceSystem::Instance()->GetTextureShared("ball"));
		playerRenderer->SetPixelSize(32, 32);

		auto playerCamera = gameObject->AddComponent<Engine::CameraComponent>();
		playerCamera->SetWindow(&Engine::RenderSystem::Instance()->GetMainWindow());
		playerCamera->SetBaseResolution(1280, 720);

		auto transform = gameObject->GetComponent<Engine::TransformComponent>();
		transform->SetWorldPosition(startPosition.x, startPosition.y);

		auto body = gameObject->AddComponent<Engine::RigidbodyComponent>();
		body->SetLinearDamping(20.f); 

		gameObject->AddComponent<Engine::SpriteColliderComponent>();
		gameObject->AddComponent<Engine::InputComponent>();
	}

	Engine::GameObject* Player::GetGameObject() { return gameObject; }
}