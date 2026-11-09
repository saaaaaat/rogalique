#include "pch.h"
#include "PlayerControllerComponent.h"
#include "GameOrchestrator.h"
#include "TransformComponent.h"
#include "HealthComponent.h"
#include "CameraComponent.h"
#include "Logger.h"
#include <SFML/Window/Keyboard.hpp>
#include <cmath>
#include <cassert>

using namespace Engine;

namespace RogaliqueGame
{
    PlayerControllerComponent::PlayerControllerComponent(GameObject* obj)
        : Component(obj)
    {
        assert(obj != nullptr);
        LOG_INFO("[Player] Controller attached, stamina=" + std::to_string((int)stamina));
    }

    static Engine::Vector2Df TryMoveWithSlide(const RoguelikeMaze& maze, Engine::Vector2Df oldPos, Engine::Vector2Df newPos, float radius)
    {
        if (maze.IsWalkableWorld(newPos.x, newPos.y, radius)) return newPos;
        if (maze.IsWalkableWorld(newPos.x, oldPos.y, radius)) return { newPos.x, oldPos.y };
        if (maze.IsWalkableWorld(oldPos.x, newPos.y, radius)) return { oldPos.x, newPos.y };
        return oldPos;
    }

    void PlayerControllerComponent::Update(float dt)
    {
        auto* t = GetGameObject()->GetComponent<TransformComponent>();
        auto* h = GetGameObject()->GetComponent<HealthComponent>();
        if (!t || !h) return;

        auto& orch = GameOrchestrator::Instance();

        if (h->IsDead())
        {
            if (!orch.IsGameOver())
            {
                LOG_ERROR("[Player] DEAD. Game over.");
                orch.SetGameOver(false);
            }
            return;
        }
        if (orch.IsGameOver()) return;

        Engine::Vector2Df dir = { 0.f, 0.f };
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))    dir.y += 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))  dir.y -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  dir.x -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) dir.x += 1.f;

        if (dir.x != 0.f || dir.y != 0.f)
        {
            float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
            dir.x /= len; dir.y /= len;
        }

        bool wantSprint = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)
            || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift);

        bool speedBoost = orch.speedBoostTimer > 0.f;
        bool sprinting = wantSprint && (stamina > 0.f || speedBoost);

        float speed = sprinting ? SPRINT_SPEED : WALK_SPEED;
        if (speedBoost) speed *= 1.5f;

        if (sprinting && !speedBoost)
        {
            stamina -= STAMINA_DRAIN * dt;
            if (stamina < 0.f) stamina = 0.f;
        }
        else
        {
            stamina += STAMINA_REGEN * dt;
            if (stamina > STAMINA_MAX) stamina = STAMINA_MAX;
        }

        if (dir.x != 0.f || dir.y != 0.f)
        {
            Engine::Vector2Df oldPos = t->GetWorldPosition();
            Engine::Vector2Df newPos = oldPos + dir * (speed * dt);
            Engine::Vector2Df actual = TryMoveWithSlide(orch.GetMaze(), oldPos, newPos, PLAYER_RADIUS);
            t->SetWorldPosition(actual.x, actual.y);
        }

        int block = orch.GetMaze().BlockAt(t->GetWorldPosition().x, t->GetWorldPosition().y);
        if (block != lastBlock)
        {
            lastBlock = block;
            Engine::Vector2Df center = orch.GetMaze().BlockCenterWorld(block);
            if (auto* cam = GetGameObject()->GetComponent<CameraComponent>())
                cam->SetCenter(center.x, center.y);
            LOG_INFO("[Player] Entered block " + std::to_string(block));
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) orch.TryUseClone();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) orch.TryUseSword();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) orch.TryUseSpeed();

        logTimer -= dt;
        if (logTimer <= 0.f)
        {
            logTimer = 1.f;
            LOG_INFO("[Player] HP=" + std::to_string(h->GetHp()) + "/" + std::to_string(h->GetMaxHp())
                + " ARM=" + std::to_string(h->GetArmor()) + "/" + std::to_string(h->GetMaxArmor())
                + " STA=" + std::to_string((int)stamina)
                + " Items{C:" + std::to_string(orch.hasCloneItem)
                + " S:" + std::to_string(orch.hasSwordItem)
                + " U:" + std::to_string(orch.hasSpeedItem)
                + " K:" + std::to_string(orch.hasKey) + "}"
                + " Boost=" + std::to_string(orch.speedBoostTimer > 0.f ? 1 : 0));
        }
    }
}