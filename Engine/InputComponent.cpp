#include "pch.h"
#include "InputComponent.h"
#include "RigidbodyComponent.h"

namespace Engine
{
	InputComponent::InputComponent(GameObject* gameObject) : Component(gameObject) {}

	void InputComponent::Update(float deltaTime)
	{
		verticalAxis = 0.f;
		horizontalAxis = 0.f;

	

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) verticalAxis += 1.0f;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) verticalAxis -= 1.0f;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) horizontalAxis += 1.0f;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) horizontalAxis -= 1.0f;

		auto rigidbody = gameObject->GetComponent<RigidbodyComponent>();
		if (rigidbody)
		{
			float speed = 150.0f;
			Vector2Df velocity(horizontalAxis * speed, verticalAxis * speed);
			rigidbody->SetLinearVelocity(velocity);
		}
	}

	void InputComponent::Render() {}

	float InputComponent::GetHorizontalAxis() const { return horizontalAxis; }
	float InputComponent::GetVerticalAxis() const { return verticalAxis; }
}