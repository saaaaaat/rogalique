#include <SFML/Graphics.hpp>
#include <windows.h>
#include "Engine.h"
#include "RenderSystem.h"
#include "ResourceSystem.h"
#include "GameWorld.h"
#include "HealthComponent.h"
#include "Logger.h"
#include "GameOrchestrator.h"
#include "PlayerControllerComponent.h"
#include "BossComponent.h"
#include <string>
#include <cstdlib>
#include <ctime>

using namespace RogaliqueGame;
using namespace Engine;

namespace
{
    void setupLogger()
    {
        auto logger = std::make_shared<::Engine::Logger>();
        logger->addSink(std::make_shared<::Engine::ConsoleSink>());
        logger->addSink(std::make_shared<::Engine::FileSink>("log.txt"));
        ::Engine::LoggerRegistry::getInstance().registerLogger("global", logger);
        ::Engine::LoggerRegistry::getInstance().setDefaultLogger(logger);
    }
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    setupLogger();
    std::srand((unsigned)std::time(nullptr));

    const unsigned int W = 1280, H = 720;

    RenderSystem::Instance()->SetMainWindow(new sf::RenderWindow(sf::VideoMode(W, H), "Roguelike - Boss Update"));
    ResourceSystem::Instance()->LoadFont("default", "Resources/Fonts/Roboto-Regular.ttf");
    const sf::Font* hudFont = ResourceSystem::Instance()->GetFont("default");

    sf::Text hudText, gameOverText, subText, invText;
    if (hudFont) {
        hudText.setFont(*hudFont); gameOverText.setFont(*hudFont);
        subText.setFont(*hudFont); invText.setFont(*hudFont);
    }


    hudText.setCharacterSize(22); hudText.setFillColor(sf::Color::White); hudText.setPosition(15.f, 15.f);
    hudText.setOutlineColor(sf::Color::Black); hudText.setOutlineThickness(2.f);


    sf::RectangleShape staminaBg(sf::Vector2f(220.f, 14.f));
    staminaBg.setFillColor(sf::Color(40, 40, 40));
    staminaBg.setOutlineColor(sf::Color::White);
    staminaBg.setOutlineThickness(1.f);
    staminaBg.setPosition(15.f, 115.f);

    sf::RectangleShape staminaFg(sf::Vector2f(220.f, 14.f));
    staminaFg.setFillColor(sf::Color(80, 220, 255));
    staminaFg.setPosition(15.f, 115.f);

    gameOverText.setCharacterSize(72); gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setOutlineColor(sf::Color::Black); gameOverText.setOutlineThickness(3.f);

    subText.setString("Press Enter to restart"); subText.setCharacterSize(32);
    subText.setFillColor(sf::Color::White); subText.setOutlineColor(sf::Color::Black);

    invText.setCharacterSize(24); invText.setFillColor(sf::Color::White);
    invText.setPosition(460.f, 160.f);

    auto& orch = GameOrchestrator::Instance();
    orch.StartLevel();

    ::Engine::Engine::Instance()->SetUpdateFunction([&](float dt) {
        orch.Update(dt);

        static bool num1WasPressed = false;
        bool num1Pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Num1);
        if (num1Pressed && !num1WasPressed) { orch.inventoryOpen = !orch.inventoryOpen; }
        num1WasPressed = num1Pressed;
        });

    ::Engine::Engine::Instance()->SetRenderFunction([&](sf::RenderWindow& window) {
        auto worldView = window.getView();
        window.setView(sf::View(sf::FloatRect(0.f, 0.f, (float)W, (float)H)));

        ::Engine::GameObject* player = orch.GetPlayer();
        PlayerControllerComponent* pc = nullptr;
        ::Engine::HealthComponent* ph = nullptr;

        if (player) {
            pc = player->GetComponent<PlayerControllerComponent>();
            ph = player->GetComponent<::Engine::HealthComponent>();
        }

        if (pc && ph) {

            std::string info =
                "HP: " + std::to_string(ph->GetHp()) + "/" + std::to_string(ph->GetMaxHp())
                + "   ARM: " + std::to_string(ph->GetArmor())
                + "\nStamina: " + std::to_string((int)pc->GetStamina())
                + "\n[Press 1 for inventory]";

            hudText.setString(info);
            window.draw(hudText);


            float pct = pc->GetStamina() / pc->GetStaminaMax();
            staminaFg.setSize(sf::Vector2f(220.f * pct, 14.f));
            staminaFg.setFillColor(orch.speedBoostTimer > 0.f ? sf::Color(255, 200, 0) : sf::Color(80, 220, 255));
            window.draw(staminaBg);
            window.draw(staminaFg);
        }


        if (orch.inventoryOpen) {
            sf::RectangleShape panel(sf::Vector2f(380.f, 460.f));
            panel.setFillColor(sf::Color(20, 20, 20, 240));
            panel.setOutlineColor(sf::Color::White);
            panel.setOutlineThickness(2.f);
            panel.setPosition(450.f, 130.f);
            window.draw(panel);

            std::string invStr = " INVENTORY \n\n";
            invStr += "[Q] Clone: " + std::string(orch.hasCloneItem ? "YES" : "NO") + "\n";
            invStr += "[E] Sword: " + std::string(orch.hasSwordItem ? "YES" : "NO") + "\n";
            invStr += "[R] Speed: " + std::string(orch.hasSpeedItem ? "YES" : "NO") + "\n";
            invStr += "[F] Health: " + std::string(orch.hasHealthPotion ? "YES" : "NO") + "\n";
            invStr += "[G] Invis: " + std::string(orch.hasInvisibilityPotion ? "YES" : "NO") + "\n\n";
            invStr += " Keys \n";
            invStr += "Half Key 1: " + std::string(orch.hasHalfKey1 ? "YES" : "NO") + "\n";
            invStr += "Half Key 2: " + std::string(orch.hasHalfKey2 ? "YES" : "NO") + "\n";
            invStr += "Purple Key: " + std::string(orch.hasPurpleKey ? "YES" : "NO") + "\n";

            invText.setString(invStr);
            window.draw(invText);
        }

        if (orch.IsGameOver())
        {
            sf::RectangleShape overlay(sf::Vector2f((float)W, (float)H));
            overlay.setFillColor(sf::Color(0, 0, 0, 200));
            window.draw(overlay);
            gameOverText.setString(orch.IsWon() ? "YOU ESCAPED" : "YOU DIED");
            gameOverText.setFillColor(orch.IsWon() ? sf::Color::Green : sf::Color::Red);
            sf::FloatRect b = gameOverText.getLocalBounds();
            gameOverText.setOrigin(b.left + b.width * 0.5f, b.top + b.height * 0.5f);
            gameOverText.setPosition(W * 0.5f, H * 0.5f - 40.f);
            window.draw(gameOverText);
            sf::FloatRect b2 = subText.getLocalBounds();
            subText.setOrigin(b2.left + b2.width * 0.5f, b2.top + b2.height * 0.5f);
            subText.setPosition(W * 0.5f, H * 0.5f + 50.f);
            window.draw(subText);
        }


        if (orch.isBossRoom)
        {
            static sf::Text bossText;
            static bool bossTextInit = false;
            if (!bossTextInit && hudFont)
            {
                bossText.setFont(*hudFont);
                bossText.setCharacterSize(28);
                bossText.setFillColor(sf::Color::Yellow);
                bossText.setOutlineColor(sf::Color::Black);
                bossText.setOutlineThickness(2.f);
                bossTextInit = true;
            }

            ::Engine::GameObject* bossObj = nullptr;
            for (auto* o : ::Engine::GameWorld::Instance()->GetGameObjects())
            {
                if (o->GetName() == "Boss") { bossObj = o; break; }
            }
            if (bossObj)
            {
                if (auto* bc = bossObj->GetComponent<BossComponent>())
                {
                    std::string s;
                    if (bc->IsDefeated())
                    {
                        s = "BOSS DEFEATED +Half Key 2. Returning to maze...";
                        bossText.setFillColor(sf::Color::Green);
                    }
                    else
                    {
                        s = "BOSS  Phase " + std::to_string(bc->GetPhase()) + " / 2"
                            + "   Time: " + std::to_string((int)bc->GetPhaseTimer()) + "s";
                    }
                    bossText.setString(s);
                    sf::FloatRect b = bossText.getLocalBounds();
                    bossText.setOrigin(b.left + b.width * 0.5f, b.top + b.height * 0.5f);
                    bossText.setPosition(W * 0.5f, 40.f);
                    window.draw(bossText);
                }
            }
        }
        window.setView(worldView);
        });

    ::Engine::Engine::Instance()->Run();
    return 0;
}