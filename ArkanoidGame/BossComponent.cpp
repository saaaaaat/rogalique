#include "pch.h"
#include "BossComponent.h"
#include "GameOrchestrator.h"
#include "GameWorld.h"
#include "TransformComponent.h"
#include "HealthComponent.h"
#include "CircleRendererComponent.h"
#include "ProjectileComponent.h"
#include "LaserComponent.h"
#include "Logger.h"
#include <cmath>
#include <cstdlib>
#include <cassert>

using namespace Engine;

namespace RogaliqueGame {

    static constexpr float ARENA_L = 300.f;
    static constexpr float ARENA_R = 980.f;
    static constexpr float ARENA_T = 160.f;
    static constexpr float ARENA_B = 560.f;
    static constexpr float ARENA_CX = 640.f;
    static constexpr float ARENA_CY = 360.f;

    BossComponent::BossComponent(GameObject* obj) : Component(obj) {
        assert(obj != nullptr && "BossComponent: null GameObject");
        LOG_INFO("BOSS: Phase 1  — survive 30 seconds!");
    }

    void BossComponent::Update(float dt) {
        if (defeated) return;
        if (GameOrchestrator::Instance().IsGameOver()) return;

        phaseTimer -= dt;
        if (phase == 1)       UpdatePhase1(dt);
        else if (phase == 2)  UpdatePhase2(dt);

        if (phaseTimer <= 0.f) {
            if (phase == 1) {
                phase = 2;
                phaseTimer = PHASE_DURATION;
                attackTimer = 0.8f;
                lastPattern = -1;
                LOG_INFO("BOSS: Phase 2  — survive 30 seconds!");
            }
            else if (phase == 2)
            {
                Defeat();
            }
        }
    }

    void BossComponent::UpdatePhase1(float dt) {
        attackTimer -= dt;
        if (attackTimer <= 0.f)
        {
            FireLaserPattern();
            attackTimer = LASER_CYCLE;
        }
    }

    void BossComponent::UpdatePhase2(float dt) {
        attackTimer -= dt;
        if (attackTimer <= 0.f) {
            FireBulletWave();
            attackTimer = BULLET_CYCLE;
        }
    }

    void BossComponent::FireLaserPattern() {
        static constexpr int NUM = 6;
        int p;
        do { p = std::rand() % NUM; } while (p == lastPattern);
        lastPattern = p;
        switch (p) {
        case 0: SpawnHBar();      break;
        case 1: SpawnVBar();      break;
        case 2: SpawnCross();     break;
        case 3: SpawnDoubleH();   break;
        case 4: SpawnSafeStrip(); break;
        case 5: SpawnDiagonalX(); break;
        }
    }

    void BossComponent::SpawnHBar() {
        SpawnLaser({ ARENA_CX, ARENA_CY },
            { ARENA_R - ARENA_L, 150.f });
        LOG_INFO("BOSS: dodge up/down");
    }

    void BossComponent::SpawnVBar() {
        SpawnLaser({ ARENA_CX, ARENA_CY },
            { 150.f, ARENA_B - ARENA_T });
        LOG_INFO("BOSS:dodge left/right");
    }

    void BossComponent::SpawnCross() {
        SpawnLaser({ ARENA_CX, ARENA_CY }, { ARENA_R - ARENA_L, 150.f });
        SpawnLaser({ ARENA_CX, ARENA_CY }, { 150.f, ARENA_B - ARENA_T });
        LOG_WARN("BOSS:hide in a corner!");
    }

    void BossComponent::SpawnDoubleH() {
        SpawnLaser({ ARENA_CX, ARENA_T + 90.f }, { ARENA_R - ARENA_L, 100.f });
        SpawnLaser({ ARENA_CX, ARENA_B - 90.f }, { ARENA_R - ARENA_L, 100.f });
        LOG_INFO("BOSS: stay in the middle strip");
    }

    void BossComponent::SpawnSafeStrip() {

        const float stripHalf = 60.f;
        if (std::rand() % 2 == 0) {

            float safeY = 240.f + (float)(std::rand() % 241);
            float topEnd = safeY - stripHalf;
            float botBeg = safeY + stripHalf;
            if (topEnd > ARENA_T + 5.f) {
                SpawnLaser({ ARENA_CX, (ARENA_T + topEnd) * 0.5f },
                    { ARENA_R - ARENA_L, topEnd - ARENA_T },
                    0.f, 1.2f, 0.4f, 35);
            }
            if (botBeg < ARENA_B - 5.f) {
                SpawnLaser({ ARENA_CX, (botBeg + ARENA_B) * 0.5f },
                    { ARENA_R - ARENA_L, ARENA_B - botBeg },
                    0.f, 1.2f, 0.4f, 35);
            }
            LOG_WARN("BOSS: SAFE STRIP  at y=" + std::to_string((int)safeY));
        }
        else {
            float safeX = 580.f + (float)(std::rand() % 121);
            float leftEnd = safeX - stripHalf;
            float rightBeg = safeX + stripHalf;
            if (leftEnd > ARENA_L + 5.f) {
                SpawnLaser({ (ARENA_L + leftEnd) * 0.5f, ARENA_CY },
                    { leftEnd - ARENA_L, ARENA_B - ARENA_T },
                    0.f, 1.2f, 0.4f, 35);
            }
            if (rightBeg < ARENA_R - 5.f) {
                SpawnLaser({ (rightBeg + ARENA_R) * 0.5f, ARENA_CY },
                    { ARENA_R - rightBeg, ARENA_B - ARENA_T },
                    0.f, 1.2f, 0.4f, 35);
            }
            LOG_WARN("BOSS: SAFE STRIP  at x=" + std::to_string((int)safeX));
        }
    }

    void BossComponent::SpawnDiagonalX() {

        const float angleDeg = 32.f;
        const float len = 1100.f;
        const float width = 120.f;
        SpawnLaser({ ARENA_CX, ARENA_CY }, { len, width }, angleDeg);
        SpawnLaser({ ARENA_CX, ARENA_CY }, { len, width }, -angleDeg);

    }

    void BossComponent::SpawnLaser(sf::Vector2f center, sf::Vector2f size,
        float angleDeg, float warn,
        float active, int dmg) {
        auto* laser = GameWorld::Instance()->CreateGameObject("BossLaser");
        if (!laser) { LOG_ERROR("Failed to spawn laser"); return; }
        laser->AddComponent<LaserComponent>(center, size, angleDeg, warn, active, dmg);
    }

    void BossComponent::FireBulletWave() {
        auto* t = GetGameObject()->GetComponent<TransformComponent>();
        if (!t) return;
        auto bossPos = t->GetWorldPosition();


        float spawnAngle = (float)(std::rand() % 360) * 3.14159265f / 180.f;
        float spawnRadius = 40.f + (float)(std::rand() % 41);
        sf::Vector2f origin(
            bossPos.x + std::cos(spawnAngle) * spawnRadius,
            bossPos.y + std::sin(spawnAngle) * spawnRadius
        );


        float baseAngle = (float)(std::rand() % 360) * 3.14159265f / 180.f;

        const int   count = 12;
        const float speed = 260.f;
        const float pi2 = 2.f * 3.14159265f;

        for (int i = 0; i < count; ++i) {
            auto* b = GameWorld::Instance()->CreateGameObject("Bullet");
            if (!b) { LOG_ERROR("Failed to spawn bullet"); continue; }
            b->AddComponent<CircleRendererComponent>(8.f, sf::Color::Yellow);
            if (auto* bt = b->GetComponent<TransformComponent>())
                bt->SetWorldPosition(origin.x, origin.y);

            float angle = baseAngle + i * (pi2 / count);
            Engine::Vector2Df dir = { std::cos(angle) * speed, std::sin(angle) * speed };
            b->AddComponent<ProjectileComponent>(dir, 4.f);
        }

        LOG_INFO("BOSS: Bullet wave — origin=(" + std::to_string((int)origin.x)
            + "," + std::to_string((int)origin.y)
            + ") baseAngle=" + std::to_string((int)(baseAngle * 180.f / 3.14159265f)) + "deg");
    }

    void BossComponent::Defeat() {
        if (defeated) return;
        defeated = true;
        LOG_INFO("BOSS DEFEATED Half Key 2 granted.");

        if (auto* r = GetGameObject()->GetComponent<CircleRendererComponent>())
            r->SetColor(sf::Color(80, 80, 80));

        auto& orch = GameOrchestrator::Instance();
        orch.hasHalfKey2 = true;
        orch.bossDefeated = true;

    }
}