#include "pch.h"
#include "LaserComponent.h"
#include "GameOrchestrator.h"
#include "GameWorld.h"
#include "TransformComponent.h"
#include "HealthComponent.h"
#include "RenderSystem.h"
#include "Logger.h"
#include <cmath>
#include <cassert>

using namespace Engine;

namespace RogaliqueGame {
    LaserComponent::LaserComponent(GameObject* obj,
        sf::Vector2f center, sf::Vector2f size,
        float angleDeg, float warningTime, float activeTime, int damage)
        : Component(obj), center(center), size(size), angle(angleDeg),
        warningTime(warningTime), activeTime(activeTime), damage(damage)
    {
        assert(obj != nullptr && "LaserComponent: null GameObject");
        assert(size.x > 0.f && size.y > 0.f && "LaserComponent: invalid size");
        assert(warningTime > 0.f && "LaserComponent: warningTime must be > 0");
        assert(activeTime > 0.f && "LaserComponent: activeTime must be > 0");
        assert(damage >= 0 && "LaserComponent: damage cannot be negative");

        shape.setSize(size);
        shape.setOrigin(size.x * 0.5f, size.y * 0.5f);
        shape.setPosition(center);
        shape.setRotation(angle);
        shape.setFillColor(sf::Color(255, 0, 0, 90));
        shape.setOutlineColor(sf::Color(255, 60, 60));
        shape.setOutlineThickness(4.f);

    }

    void LaserComponent::Update(float dt) {
        timer += dt;

        if (state == State::Warning) {
            if (timer >= warningTime) {
                state = State::Active;
                timer = 0.f;
                shape.setFillColor(sf::Color(255, 20, 20, 240));
                shape.setOutlineThickness(0.f);
            }
        }
        else if (state == State::Active) {
            if (!hasDamagedPlayer && !GameOrchestrator::Instance().bossDefeated) {
                for (auto* o : GameWorld::Instance()->GetGameObjects()) {
                    if (o->GetName() != "Player") continue;
                    auto* t = o->GetComponent<TransformComponent>();
                    auto* h = o->GetComponent<HealthComponent>();
                    if (!t || !h || h->IsDead()) break;
                    auto pos = t->GetWorldPosition();
                    if (ContainsPoint(pos.x, pos.y)) {
                        int dealt = h->TakeDamage(damage);
                        hasDamagedPlayer = true;
                        LOG_WARN("Player hit by laser Damage: " + std::to_string(dealt));
                    }
                    break;
                }
            }
            if (timer >= activeTime) {
                state = State::Done;
                GameWorld::Instance()->DestroyGameObject(GetGameObject());
            }
        }
    }


    bool LaserComponent::ContainsPoint(float px, float py) const {
        if (std::abs(angle) < 0.01f) {
            return px >= center.x - size.x * 0.5f && px <= center.x + size.x * 0.5f
                && py >= center.y - size.y * 0.5f && py <= center.y + size.y * 0.5f;
        }
        float rad = angle * 3.14159265f / 180.f;
        float c = std::cos(rad), s = std::sin(rad);
        float dx = px - center.x, dy = py - center.y;
        float rx = dx * c + dy * s;
        float ry = -dx * s + dy * c;
        return std::abs(rx) <= size.x * 0.5f && std::abs(ry) <= size.y * 0.5f;
    }

    void LaserComponent::Render() {
        if (state != State::Done)
            RenderSystem::Instance()->Render(shape);
    }
}