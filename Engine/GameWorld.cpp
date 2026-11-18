#include "pch.h"
#include "GameWorld.h"
#include "Logger.h"
#include <algorithm>

namespace Engine
{
	GameWorld* GameWorld::Instance()
	{
		static GameWorld world;
		return &world;
	}

	void GameWorld::Update(float deltaTime)
	{
		for (int i = 0; i < gameObjects.size(); i++)
		{
			gameObjects[i]->Update(deltaTime);
		}
	}
	void GameWorld::FixedUpdate(float deltaTime)
	{
		fixedCounter += deltaTime;
		if (fixedCounter > PhysicsSystem::Instance()->GetFixedDeltaTime())
		{
			fixedCounter -= PhysicsSystem::Instance()->GetFixedDeltaTime();
			PhysicsSystem::Instance()->Update();
		}
	}
	void GameWorld::Render()
	{
		for (int i = 0; i < gameObjects.size(); i++)
		{
			gameObjects[i]->Render();
		}
	}
	void GameWorld::LateUpdate()
	{
		if (markedToDestroyGameObjects.empty()) return;

		std::vector<GameObject*> toDestroy = markedToDestroyGameObjects;
		markedToDestroyGameObjects.clear();

		for (auto* obj : toDestroy)
		{
			auto it = std::find(gameObjects.begin(), gameObjects.end(), obj);
			if (it != gameObjects.end())
				DestroyGameObjectImmediate(obj);
		}
	}

	GameObject* GameWorld::CreateGameObject()
	{
		GameObject* newGameObject = new GameObject();
		gameObjects.push_back(newGameObject);
		return newGameObject;
	}
	GameObject* GameWorld::CreateGameObject(std::string name)
	{
		GameObject* newGameObject = new GameObject(name);
		gameObjects.push_back(newGameObject);
		return newGameObject;
	}
	void GameWorld::DestroyGameObject(GameObject* gameObject)
	{
		markedToDestroyGameObjects.push_back(gameObject);
	}
	void GameWorld::Clear()
	{
		std::vector<GameObject*> roots;
		for (auto* obj : gameObjects)
		{
			if (!obj) continue;
			auto* t = obj->GetComponent<TransformComponent>();
			if (t && t->GetParent() == nullptr)
				roots.push_back(obj);
		}
		for (auto* root : roots)
		{
			auto it = std::find(gameObjects.begin(), gameObjects.end(), root);
			if (it != gameObjects.end())
				DestroyGameObjectImmediate(root);
		}
		gameObjects.clear();
		markedToDestroyGameObjects.clear();
		fixedCounter = 0.f;
	}

	std::vector<GameObject*> GameWorld::DetachAll()
	{
		std::vector<GameObject*> saved = gameObjects;
		gameObjects.clear();
		markedToDestroyGameObjects.clear();
		LOG_INFO("GameWorld: detached " + std::to_string(saved.size()) + " objects");
		return saved;
	}

	void GameWorld::AttachAll(const std::vector<GameObject*>& objs)
	{
		for (auto* o : objs) {
			if (o) gameObjects.push_back(o);
		}
		LOG_INFO("GameWorld: attached " + std::to_string(gameObjects.size()) + " objects");
	}

	void GameWorld::Print() const
	{
		for (auto& obj : gameObjects)
		{
			if (obj == nullptr)
			{
				continue;
			}
			if (obj->GetComponent<TransformComponent>()->GetParent() == nullptr)
			{
				obj->Print();
			}
		}
	}

	void GameWorld::DestroyGameObjectImmediate(GameObject* gameObject)
	{
		auto parent = gameObject->GetComponent<TransformComponent>()->GetParent();
		if (parent != nullptr)
		{
			parent->GetGameObject()->RemoveChild(gameObject);
		}

		for (auto transform : gameObject->GetComponentsInChildren<TransformComponent>())
		{
			GameObject* gameObjectToDelete = transform->GetGameObject();

			gameObjects.erase(std::remove_if(gameObjects.begin(), gameObjects.end(), [gameObjectToDelete](GameObject* obj) { return obj == gameObjectToDelete; }), gameObjects.end());
			markedToDestroyGameObjects.erase(std::remove_if(markedToDestroyGameObjects.begin(), markedToDestroyGameObjects.end(), [gameObjectToDelete](GameObject* obj) { return obj == gameObjectToDelete; }), markedToDestroyGameObjects.end());

			delete gameObjectToDelete;
		}
	}
}