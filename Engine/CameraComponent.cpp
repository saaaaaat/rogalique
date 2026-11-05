#include "pch.h"
#include "CameraComponent.h"
#include "Logger.h"
#include <cassert>

namespace Engine
{
    CameraComponent::CameraComponent(GameObject* gameObject) : Component(gameObject)
    {
        assert(gameObject != nullptr && "CameraComponent requires GameObject");
        view = new sf::View(sf::FloatRect(0.f, 0.f, 1280.f, -720.f));
        view->setCenter(640.f, 360.f);
        LOG_INFO("CameraComponent created (default 1280x720, Y-flipped)");
    }

    CameraComponent::~CameraComponent()
    {
        delete view;
        LOG_INFO("CameraComponent destroyed");
    }

    void CameraComponent::Update(float /*deltaTime*/)
    {
        if (!window || !view) return;

        // Применяем статичную камеру. Никаких изменений каждый кадр — камера
        // фиксирована в (640,360), 1280x720. Если пользователь применил
        // window.setView() — например, HUD — мы возвращаем нашу.
        if (window->getView().getCenter() != view->getCenter() ||
            window->getView().getSize() != view->getSize())
        {
            window->setView(*view);
        }
    }

    void CameraComponent::Render() {}

    void CameraComponent::SetBaseResolution(int width, int height)
    {
        if (width <= 0 || height <= 0) {
            LOG_ERROR("CameraComponent::SetBaseResolution: invalid size "
                + std::to_string(width) + "x" + std::to_string(height));
            return;
        }
        view->setCenter(width * 0.5f, height * 0.5f);
        view->setSize(static_cast<float>(width), -static_cast<float>(height));
        LOG_INFO("Camera base resolution set to " + std::to_string(width) + "x" + std::to_string(height));
        if (window) window->setView(*view);
    }

    void CameraComponent::SetWindow(sf::RenderWindow* newWindow)
    {
        assert(newWindow != nullptr && "CameraComponent::SetWindow(nullptr)");
        window = newWindow;
        if (window && view) window->setView(*view);
        LOG_INFO("CameraComponent window attached");
    }

    void CameraComponent::ZoomBy(float newZoom)
    {
        if (newZoom <= 0.f) {
            LOG_WARN("CameraComponent::ZoomBy: non-positive zoom ignored");
            return;
        }
        view->zoom(newZoom);
        LOG_INFO("Camera zoomed by " + std::to_string(newZoom));
    }
}