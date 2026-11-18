#pragma once
#include "RoguelikeMaze.h"
#include "PickupComponent.h"
#include "Vector.h"

namespace Engine { class GameObject; }

namespace RogaliqueGame
{
    class GameOrchestrator
    {
    public:
        static GameOrchestrator& Instance();

        void StartLevel();
        void RestartLevel();
        void Update(float dt);

        void EnterBossRoom();
        void ExitBossRoom();

        ::Engine::GameObject* GetPlayer() const { return player; }
        ::Engine::GameObject* GetCloneOrPlayer() const { return clone ? clone : player; }

        RoguelikeMaze& GetMaze() { return maze; }
        const RoguelikeMaze& GetMaze() const { return maze; }

        void SetGameOver(bool won);
        bool IsGameOver() const { return gameOver; }
        bool IsWon() const { return won; }

        void OnPickup(ItemType type);
        void TryUseClone();
        void TryUseSword();
        void TryUseSpeed();
        void TryUseHealthPotion();
        void TryUseInvisibilityPotion();

        bool inventoryOpen = false;
        bool hasCloneItem = false;
        bool hasSwordItem = false;
        bool hasSpeedItem = false;
        bool hasHealthPotion = false;
        bool hasInvisibilityPotion = false;

        bool hasHalfKey1 = false;
        bool hasHalfKey2 = false;
        bool hasPurpleKey = false;
        bool purpleKeySpawned = false;

        float speedBoostTimer = 0.f;
        float invisibilityTimer = 0.f;
        bool isBossRoom = false;

        bool  bossDefeated = false;
        float bossExitTimer = 2.0f;

    private:
        GameOrchestrator() = default;

        RoguelikeMaze maze;
        ::Engine::GameObject* player = nullptr;
        ::Engine::GameObject* chaser = nullptr;
        ::Engine::GameObject* clone = nullptr;
        ::Engine::GameObject* exit = nullptr;

        float cloneTimer = 0.f;
        float chaserRespawnTimer = 0.f;
        float savedChaserRespawnTimer = 0.f;
        float exploderTimer = 0.f;
        bool  gameOver = false;
        bool  won = false;

        ::Engine::Vector2Df savedPortalPosition = { 0.f, 0.f };
        bool hasSavedPortalPosition = false;


        std::vector<::Engine::GameObject*> savedMazeObjects;
        ::Engine::GameObject* savedPlayer = nullptr;
        ::Engine::GameObject* savedChaser = nullptr;
        ::Engine::GameObject* savedClone = nullptr;
        ::Engine::GameObject* savedExit = nullptr;
        ::Engine::GameObject* savedPortal = nullptr;
     

        void SpawnPlayer();
        void SpawnChaser();
        void SpawnExploder();
        void SpawnExit();
        void SpawnPortal();
        void SpawnItem(ItemType type);
        void SpawnBossRoomGeometry();
    };
}