#include "pch.h"
#include "CameraComponent.h"
#include "TransformComponent.h"

namespace Engine
{
	CameraComponent::CameraComponent(GameObject* gameObject) : Component(gameObject)
	{
		view = new sf::View(sf::FloatRect(0, 0, 800, -600));
		transform = gameObject->GetComponent<TransformComponent>();
	}
	CameraComponent::~CameraComponent()
	{
		delete view;
	}

	void CameraComponent::Update(float deltaTime)
	{
		if (!window || !transform) return;

		Vector2Df playerPos = transform->GetWorldPosition();
		sf::Vector2f playerPosSf = { playerPos.x, playerPos.y };

		sf::Vector2f windowSize = (sf::Vector2f)window->getSize();

	
		sf::Vector2f cameraCenter = playerPosSf;

		float mazeWidth = 30 * 128.f;
		float mazeHeight = 20 * 128.f;

		if (cameraCenter.x < windowSize.x / 2) cameraCenter.x = windowSize.x / 2;
		if (cameraCenter.y < windowSize.y / 2) cameraCenter.y = windowSize.y / 2;

		if (cameraCenter.x > mazeWidth - windowSize.x / 2) cameraCenter.x = mazeWidth - windowSize.x / 2;
		if (cameraCenter.y > mazeHeight - windowSize.y / 2) cameraCenter.y = mazeHeight - windowSize.y / 2;

		view->setCenter(cameraCenter);
		view->setRotation(0.f);
		view->setSize(1280.f, -720.f);

		window->setView(*view);
	}

	void CameraComponent::Render()
	{
		if (window == nullptr)
		{
			std::cout << "NULL window render." << std::endl;
		}
	}

	void CameraComponent::SetBaseResolution(int width, int height)
	{
		view->reset(sf::FloatRect(0, 0, width, -height));
	}
	void CameraComponent::SetWindow(sf::RenderWindow* newWindow)
	{
		window = newWindow;
	}
	void CameraComponent::ZoomBy(float newZoom)
	{
		if (newZoom <= 0)
		{
			std::cout << "Not allowed zoom lesser or equal than zero." << std::endl;
			return;
		}
		view->zoom(newZoom);
	}
}