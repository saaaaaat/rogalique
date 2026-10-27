#include "Player.h"
#include "Logger.h"
#include "HealthComponent.h"
#include "AttackComponent.h"
#include <ResourceSystem.h>


namespace RogaliqueGame
{
	Player::Player()
	{
		LOG_INFO("Создание игрока...");
		gameObject = Engine::GameWorld::Instance()->CreateGameObject("Player");

		if (gameObject == nullptr) 
		{
			LOG_ERROR("Не удалось создать gameObject для игрока!");
			return;
		}

		
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


		gameObject->AddComponent<Engine::HealthComponent>(100, 30);
		gameObject->AddComponent<Engine::AttackComponent>();
	

		LOG_INFO("Игрок успешно создан!");
	
	}

	Engine::GameObject* Player::GetGameObject() { return gameObject; }

}