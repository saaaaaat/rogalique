#include "pch.h"
#include "SpriteColliderComponent.h"
#include "TransformComponent.h" 

namespace Engine
{
	SpriteColliderComponent::SpriteColliderComponent(GameObject* gameObject) : ColliderComponent(gameObject)
	{
		auto spriteRenderer = gameObject->GetComponent<SpriteRendererComponent>();
		if (spriteRenderer == nullptr)
		{
			std::cout << "SpriteRenderer required to SpriteCollider." << std::endl;
			gameObject->RemoveComponent(this);
			return;
		}

		sprite = gameObject->GetComponent<SpriteRendererComponent>()->GetSprite();
		PhysicsSystem::Instance()->Subscribe(this);
	}
	SpriteColliderComponent::~SpriteColliderComponent()
	{
		if (&bounds != nullptr)
		{
			std::destroy_at(&bounds);
		}
		PhysicsSystem::Instance()->Unsubscribe(this);
	}

	void SpriteColliderComponent::Update(float deltaTime)
	{
		
		auto transform = GetGameObject()->GetComponent<TransformComponent>();
		if (transform && sprite)
		{
			sf::Vector2f pos = Convert<sf::Vector2f, Vector2Df>(transform->GetWorldPosition());
			sf::FloatRect spriteBounds = sprite->getGlobalBounds();

			float width = spriteBounds.width;
			float height = spriteBounds.height;

			bounds = sf::FloatRect(pos.x - width / 2.f, pos.y - height / 2.f, width, height);
		}
	}
	void SpriteColliderComponent::Render()
	{
		sf::RectangleShape rectangle(sf::Vector2f(bounds.width, bounds.height));
		rectangle.setPosition(bounds.left, bounds.top);
		rectangle.setFillColor(sf::Color::Transparent);
		rectangle.setOutlineColor(sf::Color::White);
		rectangle.setOutlineThickness(4);

		RenderSystem::Instance()->Render(rectangle);
	}
}