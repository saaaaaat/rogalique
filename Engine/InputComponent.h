#pragma once

#include "Component.h"
#include <SFML/Window.hpp> 

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

namespace Engine
{
	class  ENGINE_API InputComponent : public Component
	{
	public:
		InputComponent(GameObject* gameObject);

		void Update(float deltaTime) override;
		void Render() override;

		float GetHorizontalAxis() const;
		float GetVerticalAxis() const;
	private:
		float horizontalAxis = 0.f;
		float verticalAxis = 0.f;
	};
}
