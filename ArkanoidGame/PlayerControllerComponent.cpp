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
#include "CircleRendererComponent.h"

using namespace Engine;

namespace RogaliqueGame
{
    PlayerControllerComponent::PlayerControllerComponent(GameObject* obj) : Component(obj) {}

    static Engine::Vector2Df TryMoveWithSlide(const RoguelikeMaze& maze,
        Engine::Vector2Df oldPos, Engine::Vector2Df newPos,
        float radius, bool isBossRoom)
    {
        if (isBossRoom)
        {
            float nx = newPos.x, ny = newPos.y;
            if (nx < 324.f)  nx = 324.f;
            if (nx > 956.f) nx = 956.f;
            if (ny < 184.f)  ny = 184.f;
            if (ny > 536.f)  ny = 536.f;
            return { nx, ny };
        }
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

        if (h->IsDead()) { if (!orch.IsGameOver()) orch.SetGameOver(false); return; }
        if (orch.IsGameOver()) return;

        Engine::Vector2Df dir = { 0.f, 0.f };
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))    dir.y += 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))  dir.y -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  dir.x -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) dir.x += 1.f;

        if (dir.x != 0.f || dir.y != 0.f) {
            float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
            dir.x /= len; dir.y /= len;
        }

        bool wantSprint = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift);
        bool hasBoost = orch.speedBoostTimer > 0.f;

        bool sprinting = hasBoost || (wantSprint && !isExhausted && stamina > 0.f);

        float speed = sprinting ? SPRINT_SPEED : WALK_SPEED;
        if (hasBoost) speed *= 1.5f;

        if (sprinting && !hasBoost) {
            stamina -= STAMINA_DRAIN * dt;
            if (stamina <= 0.f) {
                stamina = 0.f;
                isExhausted = true; 
            }
        }
        else
        {
            stamina += STAMINA_REGEN * dt;
            if (stamina > STAMINA_MAX) stamina = STAMINA_MAX;

            if (isExhausted && stamina >= 30.f) {
                isExhausted = false;
            }
        }

        if (dir.x != 0.f || dir.y != 0.f) {
            auto oldPos = t->GetWorldPosition();
            auto newPos = oldPos + dir * (speed * dt);
            auto actual = TryMoveWithSlide(orch.GetMaze(), oldPos, newPos, PLAYER_RADIUS, orch.isBossRoom);
            t->SetWorldPosition(actual.x, actual.y);
        }

        if (!orch.isBossRoom) {
            int block = orch.GetMaze().BlockAt(t->GetWorldPosition().x, t->GetWorldPosition().y);
            if (block != lastBlock) {
                lastBlock = block;
                auto center = orch.GetMaze().BlockCenterWorld(block);
                if (auto* cam = GetGameObject()->GetComponent<CameraComponent>())
                    cam->SetCenter(center.x, center.y);
                LOG_INFO("[Player] Entered block " + std::to_string(block));
            }
        }


        static bool qWas = false, eWas = false, rWas = false, fWas = false, gWas = false;

        bool qNow = sf::Keyboard::isKeyPressed(sf::Keyboard::Q);
        bool eNow = sf::Keyboard::isKeyPressed(sf::Keyboard::E);
        bool rNow = sf::Keyboard::isKeyPressed(sf::Keyboard::R);
        bool fNow = sf::Keyboard::isKeyPressed(sf::Keyboard::F);
        bool gNow = sf::Keyboard::isKeyPressed(sf::Keyboard::G);

        if (qNow && !qWas) orch.TryUseClone();
        if (eNow && !eWas) orch.TryUseSword();
        if (rNow && !rWas) orch.TryUseSpeed();
        if (fNow && !fWas) orch.TryUseHealthPotion();
        if (gNow && !gWas) orch.TryUseInvisibilityPotion();

        qWas = qNow; eWas = eNow; rWas = rNow; fWas = fNow; gWas = gNow;

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
                + " H:" + std::to_string(orch.hasHealthPotion)
                + " I:" + std::to_string(orch.hasInvisibilityPotion)
                + " K1:" + std::to_string(orch.hasHalfKey1)
                + " K2:" + std::to_string(orch.hasHalfKey2)
                + " PK:" + std::to_string(orch.hasPurpleKey) + "}"
                + " Boost=" + std::to_string(orch.speedBoostTimer > 0.f ? 1 : 0)
                + " Invis=" + std::to_string(orch.invisibilityTimer > 0.f ? 1 : 0));
        }


        if (auto* r = GetGameObject()->GetComponent<CircleRendererComponent>())
        {
            if (orch.invisibilityTimer > 0.f) {
                r->SetColor(sf::Color(180, 230, 255, 100));
            }
            else {
                r->SetColor(sf::Color(80, 200, 255));
            }
        }
    }
}