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
		auto renderer = GetGameObject()->GetComponent<SpriteRendererComponent>();

		if (transform && renderer && sprite && sprite->getTexture())
		{
		
			sf::Vector2f pos = Convert<sf::Vector2f, Vector2Df>(transform->GetWorldPosition());

		
			sf::Vector2u textureSize = sprite->getTexture()->getSize();

		
			Vector2Df scale = renderer->GetScale();

		
			float width = std::abs(textureSize.x * scale.x);
			float height = std::abs(textureSize.y * scale.y);
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