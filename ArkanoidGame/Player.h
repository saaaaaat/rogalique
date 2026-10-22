#pragma once
#include "../Engine/GameObject.h"
#include "CameraComponent.h"
#include "GameWorld.h"
#include "SpriteRendererComponent.h"
#include "RenderSystem.h"
#include "InputComponent.h"
#include "RigidbodyComponent.h"
#include "SpriteColliderComponent.h"

namespace RogaliqueGame
{
	class Player
	{
	public:
		Player();
		Engine::GameObject* GetGameObject();
		sf::Vector2f startPosition; 

	private:
		Engine::GameObject* gameObject;
	};
}