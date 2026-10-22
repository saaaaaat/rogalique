#pragma once
#include "GameObject.h"
#include "PhysicsSystem.h"

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

namespace Engine
{
	class ENGINE_API GameWorld
	{
	public:
		static GameWorld* Instance();

		void Update(float deltaTime);
		void FixedUpdate(float deltaTime);
		void Render();
		void LateUpdate();

		GameObject* CreateGameObject();
		GameObject* CreateGameObject(std::string name);
		void DestroyGameObject(GameObject* gameObject);
		void Clear();

		void Print() const;

	
		const std::vector<GameObject*>& GetGameObjects() const { return gameObjects; }
	private:
		GameWorld() {}
		~GameWorld() {}

		GameWorld(GameWorld const&) = delete;
		GameWorld& operator= (GameWorld const&) = delete;

		float fixedCounter = 0.f;

		std::vector<GameObject*> gameObjects = {};
		std::vector<GameObject*> markedToDestroyGameObjects = {};

		void DestroyGameObjectImmediate(GameObject* gameObject);
	};
}