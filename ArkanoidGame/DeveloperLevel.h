#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include "Scene.h"
#include "Player.h"
#include "Enemy.h"
#include "Wall.h"
#include "Floor.h"

namespace RogaliqueGame
{
    class DeveloperLevel : public Engine::Scene
    {
    public:
        void Start() override;
        void Restart() override;
        void Stop() override;
        void Update(float timeDelta);
    private:
        std::shared_ptr<Player> player;
        std::shared_ptr<Enemy> enemy;
        std::vector<std::shared_ptr<Wall>> walls;
    };
}