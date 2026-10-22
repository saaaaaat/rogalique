#pragma once

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

namespace Engine
{
	class GameObject;

	class  ENGINE_API Component
	{
	public:
		Component(GameObject* gameObject);
		virtual ~Component();

		virtual void Update(float deltaTime) = 0;
		virtual void Render() = 0;

		GameObject* GetGameObject();
	protected:
		GameObject* gameObject;
	};
}