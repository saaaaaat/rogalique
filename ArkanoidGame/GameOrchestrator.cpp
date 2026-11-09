#include "pch.h"
#include "GameOrchestrator.h"
#include "GameWorld.h"
#include "TransformComponent.h"
#include "HealthComponent.h"
#include "CameraComponent.h"
#include "RenderSystem.h"
#include "Logger.h"
#include "CircleRendererComponent.h"
#include "MazeRendererComponent.h"
#include "PlayerControllerComponent.h"
#include "ChaseAIComponent.h"
#include "ExploderComponent.h"
#include "ExitComponent.h"
#include "PickupComponent.h"
#include <SFML/Window/Keyboard.hpp>
#include <cstdlib>
#include <cmath>
#include <algorithm>

using namespace Engine;

namespace RogaliqueGame
{
    GameOrchestrator& GameOrchestrator::Instance()
    {
        static GameOrchestrator inst;
        return inst;
    }

    void GameOrchestrator::StartLevel()
    {
        unsigned seed = (unsigned)(std::rand() ^ (unsigned)time(nullptr));
        maze.Generate(seed);

        auto* mazeObj = GameWorld::Instance()->CreateGameObject("MazeRenderer");
        mazeObj->AddComponent<MazeRendererComponent>(&maze);

        SpawnPlayer();
        SpawnChaser();
        SpawnExit();

        SpawnItem(ItemType::Clone);
        SpawnItem(ItemType::Sword);
        SpawnItem(ItemType::Speed);
        SpawnItem(ItemType::Key);

        LOG_INFO("=== Level loaded (seed=" + std::to_string(seed) + ") ===");
        LOG_INFO("Items: Clone, Sword, Speed, Key scattered in the maze");
    }

    void GameOrchestrator::RestartLevel()
    {
        LOG_INFO("=== Restarting level ===");

        GameWorld::Instance()->Clear();
        GameWorld::Instance()->SetPaused(false);

        player = nullptr;
        chaser = nullptr;
        clone = nullptr;
        exit = nullptr;
        cloneTimer = 0.f;
        chaserRespawnTimer = 0.f;
        exploderTimer = 0.f;
        speedBoostTimer = 0.f;
        gameOver = false;
        won = false;
        hasCloneItem = false;
        hasSwordItem = false;
        hasSpeedItem = false;
        hasKey = false;

        StartLevel();
    }

    void GameOrchestrator::SetGameOver(bool wonFlag)
    {
        if (gameOver) return;
        gameOver = true;
        won = wonFlag;
        GameWorld::Instance()->SetPaused(true);
        LOG_ERROR(wonFlag ? "=== YOU WIN! ===" : "=== GAME OVER ===");
        LOG_INFO("Press Enter to restart");
    }

    void GameOrchestrator::OnPickup(ItemType type)
    {
        switch (type)
        {
        case ItemType::Clone: hasCloneItem = true; LOG_INFO("Inventory: +Clone"); break;
        case ItemType::Sword: hasSwordItem = true; LOG_INFO("Inventory: +Sword"); break;
        case ItemType::Speed: hasSpeedItem = true; LOG_INFO("Inventory: +Speed"); break;
        case ItemType::Key:   hasKey = true;       LOG_INFO("Inventory: +KEY");   break;
        }
    }

    void GameOrchestrator::SpawnPlayer()
    {
        player = GameWorld::Instance()->CreateGameObject("Player");
        player->AddComponent<CircleRendererComponent>(16.f, sf::Color(80, 200, 255));
        player->AddComponent<HealthComponent>(100, 30);
        player->AddComponent<PlayerControllerComponent>();

        auto* t = player->GetComponent<TransformComponent>();
        Engine::Vector2Df spawn = maze.GlobalCellCenter(1, 13);
        t->SetWorldPosition(spawn.x, spawn.y);
        LOG_INFO("Player spawned at (" + std::to_string((int)spawn.x) + "," + std::to_string((int)spawn.y) + ")");

        auto* cam = player->AddComponent<CameraComponent>();
        cam->SetWindow(&RenderSystem::Instance()->GetMainWindow());
        cam->SetBaseResolution(1280, 720);
        cam->SetCenter(640.f, 1080.f);
    }

    void GameOrchestrator::SpawnChaser()
    {
        chaser = GameWorld::Instance()->CreateGameObject("Chaser");
        chaser->AddComponent<CircleRendererComponent>(14.f, sf::Color(220, 60, 60));
        chaser->AddComponent<HealthComponent>(100, 0);
        chaser->AddComponent<ChaseAIComponent>();

        auto* t = chaser->GetComponent<TransformComponent>();
        Engine::Vector2Df spawn = maze.GlobalCellCenter(37, 3);
        t->SetWorldPosition(spawn.x, spawn.y);
        LOG_INFO("Chaser spawned at (" + std::to_string((int)spawn.x) + "," + std::to_string((int)spawn.y) + ")");
    }

    void GameOrchestrator::SpawnExploder()
    {
        auto* e = GameWorld::Instance()->CreateGameObject("Exploder");
        e->AddComponent<CircleRendererComponent>(12.f, sf::Color(255, 140, 0));
        e->AddComponent<ExploderComponent>();

        auto* t = e->GetComponent<TransformComponent>();
        Engine::Vector2Df ppos = { 0.f, 0.f };
        if (player)
            if (auto* pt = player->GetComponent<TransformComponent>())
                ppos = pt->GetWorldPosition();

        for (int attempt = 0; attempt < 40; ++attempt)
        {
            Engine::Vector2Df cand = maze.RandomRoomCenter();
            if ((cand - ppos).GetLength() > 250.f)
            {
                t->SetWorldPosition(cand.x, cand.y);
                LOG_INFO("[Spawn] Exploder at (" + std::to_string((int)cand.x) + "," + std::to_string((int)cand.y) + ")");
                return;
            }
        }
        GameWorld::Instance()->DestroyGameObject(e);
    }

    void GameOrchestrator::SpawnExit()
    {
        exit = GameWorld::Instance()->CreateGameObject("Exit");
        exit->AddComponent<CircleRendererComponent>(18.f, sf::Color(180, 180, 40), sf::Color::White, 3.f);
        exit->AddComponent<ExitComponent>();

        auto* t = exit->GetComponent<TransformComponent>();
        Engine::Vector2Df pos = maze.GlobalCellCenter(37, 9);
        t->SetWorldPosition(pos.x, pos.y);
        LOG_INFO("Exit spawned at (" + std::to_string((int)pos.x) + "," + std::to_string((int)pos.y) + ") in block 3");
    }

    void GameOrchestrator::SpawnItem(ItemType type)
    {
        auto* item = GameWorld::Instance()->CreateGameObject("Item");
        sf::Color color;
        switch (type)
        {
        case ItemType::Clone: color = sf::Color(40, 220, 80);  break;
        case ItemType::Sword: color = sf::Color(220, 40, 40);  break;
        case ItemType::Speed: color = sf::Color(40, 80, 255);  break;
        case ItemType::Key:   color = sf::Color(255, 220, 40); break;
        }
        item->AddComponent<CircleRendererComponent>(10.f, color, sf::Color::Black, 2.f);
        item->AddComponent<PickupComponent>(type);

        auto* t = item->GetComponent<TransformComponent>();
        Engine::Vector2Df pos;
        if (type == ItemType::Key)
        {
            int b = (std::rand() % 2 == 0) ? 1 : 2;
            pos = maze.RandomRoomCenter(b);
        }
        else
        {
            pos = maze.RandomRoomCenter();
        }
        t->SetWorldPosition(pos.x, pos.y);
    }

    void GameOrchestrator::TryUseClone()
    {
        if (!hasCloneItem) return;
        if (clone) { LOG_WARN("Clone already active"); return; }
        hasCloneItem = false;

        auto* pt = player->GetComponent<TransformComponent>();
        Engine::Vector2Df pos = pt->GetWorldPosition();

        clone = GameWorld::Instance()->CreateGameObject("Clone");
        clone->AddComponent<CircleRendererComponent>(14.f, sf::Color(40, 220, 80, 180), sf::Color::White, 1.f);
        auto* ct = clone->GetComponent<TransformComponent>();
        ct->SetWorldPosition(pos.x, pos.y);
        cloneTimer = 10.f;
        LOG_INFO("[Item] Clone deployed at (" + std::to_string((int)pos.x) + "," + std::to_string((int)pos.y) + "), 10s duration");
    }

    void GameOrchestrator::TryUseSword()
    {
        if (!hasSwordItem) return;
        hasSwordItem = false;

        struct Target { Engine::GameObject* obj; float dist; };
        std::vector<Engine::GameObject*> candidates;
        if (chaser) candidates.push_back(chaser);
        for (auto* o : GameWorld::Instance()->GetGameObjects())
            if (o->GetName() == "Exploder") candidates.push_back(o);

        auto* pt = player->GetComponent<TransformComponent>();
        Engine::Vector2Df pp = pt->GetWorldPosition();

        Target best = { nullptr, 1e9f };
        for (auto* c : candidates)
        {
            auto* t = c->GetComponent<TransformComponent>();
            if (!t) continue;
            float d = (t->GetWorldPosition() - pp).GetLength();
            if (d < best.dist && d < 200.f) best = { c, d };
        }

        if (!best.obj)
        {
            LOG_WARN("[Sword] No enemy in range");
            return;
        }

        if (auto* h = best.obj->GetComponent<HealthComponent>())
        {
            h->TakeDamage(100);
            LOG_INFO("[Sword] Hit " + best.obj->GetName() + " for 100 dmg");
        }
        if (best.obj == chaser)
        {
            if (auto* ai = chaser->GetComponent<ChaseAIComponent>())
            {
                ai->Stun(5.f);
                LOG_INFO("[Sword] Chaser STUNNED for 5s");
            }
        }
        else
        {
            GameWorld::Instance()->DestroyGameObject(best.obj);
            LOG_INFO("[Sword] Exploder destroyed");
        }
    }

    void GameOrchestrator::TryUseSpeed()
    {
        if (!hasSpeedItem) return;
        hasSpeedItem = false;
        speedBoostTimer = 6.f;
        LOG_INFO("[Item] Speed boost ACTIVE for 6s (no stamina drain)");
    }

    void GameOrchestrator::Update(float dt)
    {
        if (gameOver)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
                RestartLevel();
            return;
        }

        if (chaser)
        {
            if (auto* h = chaser->GetComponent<HealthComponent>())
            {
                if (h->IsDead())
                {
                    LOG_ERROR("[Chaser] DESTROYED! Will respawn in 8s");
                    GameWorld::Instance()->DestroyGameObject(chaser);
                    chaser = nullptr;
                    chaserRespawnTimer = 8.f;
                }
            }
        }
        if (!chaser)
        {
            chaserRespawnTimer -= dt;
            if (chaserRespawnTimer <= 0.f)
            {
                SpawnChaser();
                LOG_WARN("[Chaser] Respawned!");
            }
        }

        exploderTimer += dt;
        if (exploderTimer >= 8.f)
        {
            exploderTimer = 0.f;
            SpawnExploder();
        }

        if (clone)
        {
            cloneTimer -= dt;
            if (cloneTimer <= 0.f)
            {
                LOG_INFO("[Clone] Expired");
                GameWorld::Instance()->DestroyGameObject(clone);
                clone = nullptr;
            }
        }

        if (speedBoostTimer > 0.f) speedBoostTimer -= dt;
    }
}