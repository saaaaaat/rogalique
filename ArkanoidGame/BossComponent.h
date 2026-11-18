#pragma once
#include "Component.h"
#include <SFML/Graphics.hpp>

namespace RogaliqueGame {
    class BossComponent : public ::Engine::Component {
    public:
        static constexpr float PHASE_DURATION = 30.f;
        static constexpr float LASER_CYCLE = 2.4f;
        static constexpr float BULLET_CYCLE = 1.8f;

        BossComponent(::Engine::GameObject* obj);
        void Update(float dt) override;
        void Render() override {}

        int   GetPhase()      const { return phase; }
        float GetPhaseTimer() const { return phaseTimer; }
        bool  IsDefeated()    const { return defeated; }

    private:
        void UpdatePhase1(float dt);
        void UpdatePhase2(float dt);
        void FireLaserPattern();
        void FireBulletWave();

        void SpawnLaser(sf::Vector2f center, sf::Vector2f size,
            float angleDeg = 0.f, float warn = 0.9f,
            float active = 0.4f, int dmg = 30);

        void SpawnHBar();
        void SpawnVBar();
        void SpawnCross();
        void SpawnDoubleH();
        void SpawnSafeStrip();
        void SpawnDiagonalX();
        void Defeat();

        int   phase = 1;
        float phaseTimer = PHASE_DURATION;
        float attackTimer = 2.f;
        int   lastPattern = -1;
        bool  defeated = false;
    };
}