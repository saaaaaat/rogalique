#pragma once
#include "RoguelikeMaze.h"
#include "PickupComponent.h"

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

        Engine::GameObject* GetPlayer() const { return player; }
        Engine::GameObject* GetCloneOrPlayer() const { return clone ? clone : player; }

        RoguelikeMaze& GetMaze() { return maze; }
        const RoguelikeMaze& GetMaze() const { return maze; }

        void SetGameOver(bool won);
        bool IsGameOver() const { return gameOver; }
        bool IsWon() const { return won; }

        void OnPickup(ItemType type);

       
        void TryUseClone();
        void TryUseSword();
        void TryUseSpeed();

  
        bool hasCloneItem = false;
        bool hasSwordItem = false;
        bool hasSpeedItem = false;
        bool hasKey = false;
        float speedBoostTimer = 0.f;

    private:
        GameOrchestrator() = default;

        RoguelikeMaze maze;
        Engine::GameObject* player = nullptr;
        Engine::GameObject* chaser = nullptr;
        Engine::GameObject* clone = nullptr;
        Engine::GameObject* exit = nullptr;

        float cloneTimer = 0.f;
        float chaserRespawnTimer = 0.f;
        float exploderTimer = 0.f;
        bool gameOver = false;
        bool won = false;

        void SpawnPlayer();
        void SpawnChaser();
        void SpawnExploder();
        void SpawnExit();
        void SpawnItem(ItemType type);
    };
}