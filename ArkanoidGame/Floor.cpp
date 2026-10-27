#include "Floor.h"
#include "GameWorld.h"
#include "SpriteRendererComponent.h"

namespace RogaliqueGame
{
    Floor::Floor(sf::Vector2f position, sf::Vector2f size)
    {
        gameObject = Engine::GameWorld::Instance()->CreateGameObject("Floor");

        auto renderer = gameObject->AddComponent<Engine::SpriteRendererComponent>();

       
        sf::Texture* tex = new sf::Texture();
        sf::Image img;

    
        img.create(1, 1, sf::Color(105,105, 105)); 
        tex->loadFromImage(img);

        renderer->SetTexture(*tex);

     
        renderer->SetPixelSize((int)size.x, (int)size.y);

      
        auto transform = gameObject->GetComponent<Engine::TransformComponent>();
        transform->SetWorldPosition(position.x, position.y);
    }
}