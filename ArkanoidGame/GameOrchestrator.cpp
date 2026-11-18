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
#include "PortalComponent.h"
#include "BossComponent.h"
#include "PickupComponent.h"
#include <SFML/Window/Keyboard.hpp>
#include <cstdlib>
#include <cmath>
#include <cassert>

using namespace Engine;

namespace RogaliqueGame
{

    static constexpr int ARENA_L = 300;
    static constexpr int ARENA_R = 980;
    static constexpr int ARENA_T = 160;
    static constexpr int ARENA_B = 560;

    class BossRoomRendererComponent : public Engine::Component
    {
    public:
        BossRoomRendererComponent(Engine::GameObject* obj) : Component(obj) {
            assert(obj != nullptr && "BossRoomRenderer: null obj");

            const int W = 1280, H = 720;
            sf::Image img;
            img.create(W, H, sf::Color(55, 55, 75));


            for (int x = ARENA_L; x < ARENA_R; ++x)
                for (int y = ARENA_T; y < ARENA_B; ++y)
                    img.setPixel(x, y, sf::Color(35, 35, 45));

            const sf::Color border(140, 140, 180);
            for (int x = ARENA_L; x < ARENA_R; ++x) {
                img.setPixel(x, ARENA_T, border);
                img.setPixel(x, ARENA_B - 1, border);
            }
            for (int y = ARENA_T; y < ARENA_B; ++y) {
                img.setPixel(ARENA_L, y, border);
                img.setPixel(ARENA_R - 1, y, border);
            }

            texture = new sf::Texture();
            if (!texture->loadFromImage(img))
                LOG_ERROR("BossRoomRenderer: failed to load texture");
            texture->setSmooth(false);
            sprite.setTexture(*texture);
            sprite.setPosition(0.f, 0.f);


        }

        ~BossRoomRendererComponent() override {
            delete texture;
            texture = nullptr;
        }

        void Update(float) override {}
        void Render() override {
            if (texture) Engine::RenderSystem::Instance()->Render(sprite);
        }

    private:
        sf::Texture* texture = nullptr;
        sf::Sprite sprite;
    };



    GameOrchestrator& GameOrchestrator::Instance() {
        static GameOrchestrator inst;
        return inst;
    }

    void GameOrchestrator::StartLevel() {
        isBossRoom = false;
        bossDefeated = false;
        hasSavedPortalPosition = false;
        maze.Generate((unsigned)std::rand());

        auto* mazeObj = GameWorld::Instance()->CreateGameObject("MazeRenderer");
        if (mazeObj) mazeObj->AddComponent<MazeRendererComponent>(&maze);

        SpawnPlayer();
        SpawnChaser();
        SpawnExit();
        SpawnPortal();
        SpawnItem(ItemType::Clone);
        SpawnItem(ItemType::Sword);
        SpawnItem(ItemType::Speed);
        SpawnItem(ItemType::Key1);
        SpawnItem(ItemType::HealthPotion);
        SpawnItem(ItemType::InvisibilityPotion);

        purpleKeySpawned = false;
        LOG_INFO("Level Generated ");
    }

    void GameOrchestrator::RestartLevel() {
        GameWorld::Instance()->Clear();

        for (auto* o : savedMazeObjects) delete o;
        savedMazeObjects.clear();
        savedPlayer = savedChaser = savedClone = savedExit = nullptr;

        savedPortal = nullptr;

        if (savedPortal) { delete savedPortal; savedPortal = nullptr; }

        GameWorld::Instance()->SetPaused(false);
        player = chaser = clone = exit = nullptr;
        cloneTimer = chaserRespawnTimer = exploderTimer = 0.f;
        speedBoostTimer = invisibilityTimer = 0.f;
        gameOver = won = isBossRoom = inventoryOpen = false;
        bossDefeated = false;
        bossExitTimer = 2.0f;
        hasSavedPortalPosition = false;

        hasCloneItem = hasSwordItem = hasSpeedItem = false;
        hasHealthPotion = hasInvisibilityPotion = false;
        hasHalfKey1 = hasHalfKey2 = hasPurpleKey = false;
        purpleKeySpawned = false;

        LOG_INFO(" RESTARTING LEVEL ");
        StartLevel();
    }

    void GameOrchestrator::EnterBossRoom() {
        if (isBossRoom) return;

        hasSavedPortalPosition = false;
        savedPortal = nullptr;


        for (auto* o : GameWorld::Instance()->GetGameObjects()) {
            if (o->GetName() == "Portal") {
                savedPortal = o;
                if (auto* pt = o->GetComponent<TransformComponent>()) {
                    savedPortalPosition = pt->GetWorldPosition();
                    hasSavedPortalPosition = true;
                    LOG_INFO("Portal saved at: ("
                        + std::to_string((int)savedPortalPosition.x) + ","
                        + std::to_string((int)savedPortalPosition.y) + ")");
                }
                break;
            }
        }

        savedPlayer = player;
        savedChaser = chaser;
        savedClone = clone;
        savedExit = exit;
        savedChaserRespawnTimer = chaserRespawnTimer;

        savedMazeObjects = GameWorld::Instance()->DetachAll();
        LOG_INFO("Maze preserved: " + std::to_string(savedMazeObjects.size()) + " objects");

        isBossRoom = true;
        inventoryOpen = false;
        player = chaser = clone = exit = nullptr;
        bossDefeated = false;
        bossExitTimer = 2.0f;
        speedBoostTimer = 0.f;
        invisibilityTimer = 0.f;

        SpawnBossRoomGeometry();

        SpawnPlayer();
        if (player)
        {
            if (auto* t = player->GetComponent<TransformComponent>())
                t->SetWorldPosition(640.f, 500.f);
            if (auto* cam = player->GetComponent<CameraComponent>())
                cam->SetCenter(640.f, 360.f);
        }

        auto* boss = GameWorld::Instance()->CreateGameObject("Boss");
        if (!boss) { LOG_ERROR("Failed to spawn Boss"); return; }
        boss->AddComponent<CircleRendererComponent>(50.f, sf::Color(150, 0, 150));
        if (auto* bt = boss->GetComponent<TransformComponent>())
            bt->SetWorldPosition(640.f, 360.f);
        boss->AddComponent<BossComponent>();

        LOG_ERROR("BOSS ROOM ENTERED. PREPARE TO DODGE");
    }

    void GameOrchestrator::ExitBossRoom() {
        LOG_INFO("[Boss] ExitBossRoom start");


        GameWorld::Instance()->Clear();
        isBossRoom = false;
        bossDefeated = false;
        player = chaser = clone = exit = nullptr;
        chaserRespawnTimer = savedChaserRespawnTimer;
        exploderTimer = 0.f;
        speedBoostTimer = 0.f;
        invisibilityTimer = 0.f;


        if (savedPortal) {
            auto it = std::find(savedMazeObjects.begin(), savedMazeObjects.end(), savedPortal);
            if (it != savedMazeObjects.end()) {
                savedMazeObjects.erase(it);
            }
            delete savedPortal;
            savedPortal = nullptr;
            LOG_INFO("[Boss] Portal destroyed permanently");
        }


        GameWorld::Instance()->AttachAll(savedMazeObjects);
        size_t restoredCount = savedMazeObjects.size();
        savedMazeObjects.clear();


        player = savedPlayer;
        chaser = savedChaser;
        clone = savedClone;
        exit = savedExit;
        savedPlayer = savedChaser = savedClone = savedExit = nullptr;

        if (hasSavedPortalPosition && player) {
            if (auto* pt = player->GetComponent<TransformComponent>())
                pt->SetWorldPosition(savedPortalPosition.x, savedPortalPosition.y);
            int block = maze.BlockAt(savedPortalPosition.x, savedPortalPosition.y);
            auto center = maze.BlockCenterWorld(block);
            if (auto* cam = player->GetComponent<CameraComponent>())
                cam->SetCenter(center.x, center.y);
            LOG_INFO("Player placed at portal position ("
                + std::to_string((int)savedPortalPosition.x) + ","
                + std::to_string((int)savedPortalPosition.y) + ")");
        }


        if (chaser) {
            if (auto* ct = chaser->GetComponent<TransformComponent>()) {
                auto pos = ct->GetWorldPosition();
                int gx, gy;
                if (maze.WorldToGlobal(pos.x, pos.y, gx, gy) && maze.IsWall(gx, gy)) {
                    auto center = maze.GlobalCellCenter(gx, gy);
                    ct->SetWorldPosition(center.x, center.y);
                    LOG_WARN("[Chaser] Was in wall, snapped to cell ("
                        + std::to_string(gx) + "," + std::to_string(gy) + ")");
                }
            }
        }

        LOG_INFO("Maze restored: " + std::to_string(restoredCount) + " objects");

    }

    void GameOrchestrator::SpawnBossRoomGeometry() {
        auto* room = GameWorld::Instance()->CreateGameObject("BossRoomGeometry");
        if (!room) { LOG_ERROR("Failed to spawn boss room geometry"); return; }
        room->AddComponent<BossRoomRendererComponent>();
    }

    void GameOrchestrator::Update(float dt) {
        if (gameOver) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) RestartLevel();
            return;
        }

        if (isBossRoom) {
            if (bossDefeated) {
                bossExitTimer -= dt;
                if (bossExitTimer <= 0.f) {
                    LOG_INFO("[Boss] Leaving boss room...");
                    ExitBossRoom();
                }
            }
            return;
        }

        if (chaser) {
            if (chaser->GetComponent<HealthComponent>()->IsDead()) {
                GameWorld::Instance()->DestroyGameObject(chaser);
                chaser = nullptr;
                chaserRespawnTimer = 8.f;
                LOG_WARN("[Chaser] Died Respawning in 8s...");
            }
        }
        if (!chaser) {
            chaserRespawnTimer -= dt;
            if (chaserRespawnTimer <= 0.f) { SpawnChaser(); LOG_INFO("[Chaser] Respawned!"); }
        }

        exploderTimer += dt;
        if (exploderTimer >= 8.f) { exploderTimer = 0.f; SpawnExploder(); }

        if (clone) {
            cloneTimer -= dt;
            if (cloneTimer <= 0.f) {
                GameWorld::Instance()->DestroyGameObject(clone);
                clone = nullptr;
                LOG_INFO("[Clone] Expired.");
            }
        }

        if (speedBoostTimer > 0.f)    speedBoostTimer -= dt;
        if (invisibilityTimer > 0.f)  invisibilityTimer -= dt;
    }

    void GameOrchestrator::SetGameOver(bool wonFlag) {
        if (gameOver) return;
        gameOver = true;
        won = wonFlag;
        GameWorld::Instance()->SetPaused(true);
        LOG_ERROR(wonFlag ? "YOU WIN" : "GAME OVER");
    }

    void GameOrchestrator::OnPickup(ItemType type) {
        switch (type) {
        case ItemType::Clone:              hasCloneItem = true;  LOG_INFO("Inventory: +Clone"); break;
        case ItemType::Sword:              hasSwordItem = true;  LOG_INFO("Inventory: +Sword"); break;
        case ItemType::Speed:              hasSpeedItem = true;  LOG_INFO("Inventory: +Speed"); break;
        case ItemType::Key1:               hasHalfKey1 = true;   LOG_INFO("Inventory: +Half Key 1"); break;
        case ItemType::Key2:               hasHalfKey2 = true;   LOG_INFO("Inventory: +Half Key 2"); break;
        case ItemType::PurpleKey:          hasPurpleKey = true;  LOG_INFO("Inventory: +Purple Key"); break;
        case ItemType::HealthPotion:       hasHealthPotion = true; LOG_INFO("Inventory: +Health Potion"); break;
        case ItemType::InvisibilityPotion: hasInvisibilityPotion = true; LOG_INFO("Inventory: +Invisibility Potion"); break;
        }
    }


    void GameOrchestrator::TryUseHealthPotion() {
        if (isBossRoom) { LOG_WARN("[Item] Items disabled in the boss room!"); return; }
        if (!hasHealthPotion) return;
        hasHealthPotion = false;
        if (auto* h = player->GetComponent<HealthComponent>()) h->Heal(25);
        LOG_INFO("[Item] Health Potion used(+25 HP)");
    }
    void GameOrchestrator::TryUseInvisibilityPotion() {
        if (isBossRoom) { LOG_WARN("[Item] Items disabled in the boss room!"); return; }
        if (!hasInvisibilityPotion) return;
        hasInvisibilityPotion = false;
        invisibilityTimer = 10.f;
        LOG_INFO("[Item] Invisibility active for 10s");
    }
    void GameOrchestrator::TryUseSpeed() {
        if (isBossRoom) { LOG_WARN("[Item] Items disabled in the boss room!"); return; }
        if (!hasSpeedItem) return;
        hasSpeedItem = false;
        speedBoostTimer = 6.f;
        LOG_INFO("[Item] Speed boost active for 6s");
    }
    void GameOrchestrator::TryUseClone() {
        if (isBossRoom) { LOG_WARN("[Item] Items disabled in the boss room!"); return; }
        if (!hasCloneItem || clone) return;
        hasCloneItem = false;
        auto pos = player->GetComponent<TransformComponent>()->GetWorldPosition();
        clone = GameWorld::Instance()->CreateGameObject("Clone");
        if (!clone) { LOG_ERROR("Failed to spawn Clone"); return; }
        clone->AddComponent<CircleRendererComponent>(14.f, sf::Color(40, 220, 80, 180));
        clone->GetComponent<TransformComponent>()->SetWorldPosition(pos.x, pos.y);
        cloneTimer = 10.f;
        LOG_INFO("[Item] Clone deployed");
    }
    void GameOrchestrator::TryUseSword() {
        if (isBossRoom) { LOG_WARN("[Item] Items disabled in the boss room!"); return; }
        if (!hasSwordItem) return;
        hasSwordItem = false;
        LOG_INFO("[Item] Sword used");

        auto pp = player->GetComponent<TransformComponent>()->GetWorldPosition();


        std::vector<GameObject*> targets;
        for (auto* o : GameWorld::Instance()->GetGameObjects()) {
            if (o->GetName() != "Chaser" && o->GetName() != "Exploder") continue;
            auto* t = o->GetComponent<TransformComponent>();
            if (t && (t->GetWorldPosition() - pp).GetLength() < 200.f)
                targets.push_back(o);
        }

        for (auto* target : targets) {
            auto* t = target->GetComponent<TransformComponent>();
            if (target->GetName() == "Chaser") {
                if (!purpleKeySpawned) {
                    auto* pk = GameWorld::Instance()->CreateGameObject("Item");
                    if (pk) {
                        pk->AddComponent<CircleRendererComponent>(10.f, sf::Color(128, 0, 128));
                        pk->AddComponent<PickupComponent>(ItemType::PurpleKey);
                        pk->GetComponent<TransformComponent>()
                            ->SetWorldPosition(t->GetWorldPosition().x, t->GetWorldPosition().y);
                        purpleKeySpawned = true;
                        LOG_INFO("[Sword] Chaser dropped Purple Key!");
                    }
                }
                GameWorld::Instance()->DestroyGameObject(chaser);
                chaser = nullptr;
                chaserRespawnTimer = 8.f;
                LOG_INFO("[Sword] Chaser destroyed");
            }
            else {
                GameWorld::Instance()->DestroyGameObject(target);
                LOG_INFO("[Sword] Exploder destroyed");
            }
            return;
        }
    }

    void GameOrchestrator::SpawnPlayer() {
        player = GameWorld::Instance()->CreateGameObject("Player");
        if (!player) { LOG_ERROR("Failed to spawn Player"); return; }
        player->AddComponent<CircleRendererComponent>(16.f, sf::Color(80, 200, 255));
        player->AddComponent<HealthComponent>(100, 30);
        player->AddComponent<PlayerControllerComponent>();
        auto* t = player->GetComponent<TransformComponent>();
        auto spawn = maze.GlobalCellCenter(1, 13);
        if (t) t->SetWorldPosition(spawn.x, spawn.y);
        auto* cam = player->AddComponent<CameraComponent>();
        if (cam) {
            cam->SetWindow(&RenderSystem::Instance()->GetMainWindow());
            cam->SetBaseResolution(1280, 720);
            cam->SetCenter(640.f, 1080.f);
        }
    }

    void GameOrchestrator::SpawnChaser() {
        chaser = GameWorld::Instance()->CreateGameObject("Chaser");
        if (!chaser) { LOG_ERROR("Failed to spawn Chaser"); return; }
        chaser->AddComponent<CircleRendererComponent>(14.f, sf::Color(220, 60, 60));
        chaser->AddComponent<HealthComponent>(100, 0);
        chaser->AddComponent<ChaseAIComponent>();
        auto spawn = maze.GlobalCellCenter(57, 3);
        chaser->GetComponent<TransformComponent>()->SetWorldPosition(spawn.x, spawn.y);
    }

    void GameOrchestrator::SpawnExploder() {
        auto* e = GameWorld::Instance()->CreateGameObject("Exploder");
        if (!e) { LOG_ERROR("Failed to spawn Exploder"); return; }
        e->AddComponent<CircleRendererComponent>(12.f, sf::Color(255, 140, 0));
        e->AddComponent<ExploderComponent>();
        auto pos = maze.RandomRoomCenter();
        e->GetComponent<TransformComponent>()->SetWorldPosition(pos.x, pos.y);
    }

    void GameOrchestrator::SpawnExit() {
        exit = GameWorld::Instance()->CreateGameObject("Exit");
        if (!exit) { LOG_ERROR("Failed to spawn Exit"); return; }
        exit->AddComponent<CircleRendererComponent>(18.f, sf::Color(180, 180, 40), sf::Color::White, 3.f);
        exit->AddComponent<ExitComponent>();
        auto pos = maze.GlobalCellCenter(57, 9);
        exit->GetComponent<TransformComponent>()->SetWorldPosition(pos.x, pos.y);
    }

    void GameOrchestrator::SpawnPortal() {
        auto* p = GameWorld::Instance()->CreateGameObject("Portal");
        if (!p) { LOG_ERROR("Failed to spawn Portal"); return; }
        p->AddComponent<CircleRendererComponent>(18.f, sf::Color(128, 0, 128), sf::Color::White, 3.f);
        p->AddComponent<PortalComponent>();
        auto pos = maze.RandomRoomCenter(2);
        p->GetComponent<TransformComponent>()->SetWorldPosition(pos.x, pos.y);
    }

    void GameOrchestrator::SpawnItem(ItemType type) {
        auto* item = GameWorld::Instance()->CreateGameObject("Item");
        if (!item) { LOG_ERROR("Failed to spawn Item"); return; }
        sf::Color color;
        switch (type) {
        case ItemType::Clone:              color = sf::Color(40, 220, 80);   break;
        case ItemType::Sword:              color = sf::Color(220, 40, 40);   break;
        case ItemType::Speed:              color = sf::Color(40, 80, 255);   break;
        case ItemType::Key1:               color = sf::Color(255, 220, 40);  break;
        case ItemType::HealthPotion:       color = sf::Color(255, 105, 180); break;
        case ItemType::InvisibilityPotion: color = sf::Color(255, 255, 255); break;
        default:                           color = sf::Color::White;         break;
        }
        item->AddComponent<CircleRendererComponent>(10.f, color, sf::Color::Black, 2.f);
        item->AddComponent<PickupComponent>(type);
        auto pos = (type == ItemType::Key1) ? maze.RandomRoomCenter(4) : maze.RandomRoomCenter();
        item->GetComponent<TransformComponent>()->SetWorldPosition(pos.x, pos.y);
    }
}