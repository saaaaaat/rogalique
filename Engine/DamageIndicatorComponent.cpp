#include "pch.h"
#include "DamageIndicatorComponent.h"
#include "TransformComponent.h"
#include "ResourceSystem.h"
#include "RenderSystem.h"
#include "GameWorld.h"
#include "Logger.h"
#include <cassert>

namespace Engine
{
    DamageIndicatorComponent::DamageIndicatorComponent(GameObject* gameObject,
        const std::string& text,
        sf::Color color,
        float lifetime)
        : Component(gameObject), lifetime(lifetime)
    {
        assert(lifetime > 0.f && "DamageIndicator: lifetime must be > 0");

        const sf::Font* font = ResourceSystem::Instance()->GetFont("default");
        assert(font != nullptr && "DamageIndicator: 'default' font not loaded!");
        if (font) textObject.setFont(*font);

        textObject.setString(text);
        textObject.setCharacterSize(32);
        textObject.setStyle(sf::Text::Bold);
        textObject.setFillColor(color);
        textObject.setOutlineColor(sf::Color::Black);
        textObject.setOutlineThickness(2.f);
        textObject.setScale(1.f, -1.f); // компенсируем Y-flip камеры

        auto b = textObject.getLocalBounds();
        textObject.setOrigin(b.left + b.width * 0.5f, b.top + b.height * 0.5f);

        LOG_INFO("DamageIndicator '" + text + "' spawned (lifetime="
            + std::to_string(lifetime) + "s)");
    }

    void DamageIndicatorComponent::Update(float deltaTime)
    {
        elapsed += deltaTime;
        if (elapsed >= lifetime)
        {
            LOG_INFO("DamageIndicator expired, destroying GameObject");
            GameWorld::Instance()->DestroyGameObject(GetGameObject());
            return;
        }

        if (auto* t = GetGameObject()->GetComponent<TransformComponent>())
            t->MoveBy(velocity * deltaTime);

        float alpha = 1.f - elapsed / lifetime;
        if (alpha < 0.f) alpha = 0.f;
        sf::Uint8 a = static_cast<sf::Uint8>(255.f * alpha);

        sf::Color fill = textObject.getFillColor();
        fill.a = a;
        textObject.setFillColor(fill);

        sf::Color outline = textObject.getOutlineColor();
        outline.a = a;
        textObject.setOutlineColor(outline);
    }

    void DamageIndicatorComponent::Render()
    {
        auto* t = GetGameObject()->GetComponent<TransformComponent>();
        if (!t) return;
        auto pos = t->GetWorldPosition();
        textObject.setPosition(pos.x, pos.y);
        RenderSystem::Instance()->Render(textObject);
    }
}