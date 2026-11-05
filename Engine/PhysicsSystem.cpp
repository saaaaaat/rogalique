#include "pch.h"
#include "PhysicsSystem.h"
#include "TransformComponent.h"
#include "Logger.h"
#include <cassert>

namespace Engine
{
	PhysicsSystem* PhysicsSystem::Instance()
	{
		static PhysicsSystem physicsSystem;
		return &physicsSystem;
	}

	float PhysicsSystem::GetFixedDeltaTime() const
	{
		return fixedDeltaTime;
	}

	void PhysicsSystem::Update()
	{
		for (int i = 0; i < colliders.size(); i++)
		{
			auto body = colliders[i]->GetGameObject()->GetComponent<RigidbodyComponent>();

			// Добавил проверку body, чтобы не крашнулось, если коллайдер есть, а Rigidbody нет
			if (body && body->GetKinematic()) continue;

			for (int j = 0; j < colliders.size(); j++)
			{
				if (j == i) continue;

				sf::FloatRect intersection;
				if (colliders[i]->bounds.intersects(colliders[j]->bounds, intersection))
				{
					std::string name1 = colliders[i]->GetGameObject()->GetName();
					std::string name2 = colliders[j]->GetGameObject()->GetName();

					if ((name1 == "Player" && name2 == "Enemy") || (name1 == "Enemy" && name2 == "Player"))
					{
						auto* t1 = colliders[i]->GetGameObject()->GetComponent<TransformComponent>();
						auto* t2 = colliders[j]->GetGameObject()->GetComponent<TransformComponent>();
						if (t1 && t2)
						{
							Vector2Df p1 = t1->GetWorldPosition();
							Vector2Df p2 = t2->GetWorldPosition();
							Vector2Df d = { p1.x - p2.x, p1.y - p2.y };
							float len = std::sqrt(d.x * d.x + d.y * d.y);
							if (len < 0.001f) { d = { 1.f, 0.f }; len = 1.f; }
							d.x /= len; d.y /= len;
							float push = std::min(intersection.width, intersection.height) * 0.55f + 0.5f;

							t1->MoveBy(d * push);
							t2->MoveBy({ -d.x * push, -d.y * push });
							LOG_WARN("Player<->Enemy overlap - pushed apart by " + std::to_string(push));
						}
						continue; // Пропускаем остальную физику для этой пары
					}

					if (colliders[i]->isTrigger != colliders[j]->isTrigger)
					{
						if (triggersEnteredPair.find(colliders[i]) == triggersEnteredPair.end() && triggersEnteredPair.find(colliders[j]) == triggersEnteredPair.end())
						{
							Trigger trigger(colliders[i], colliders[j]);
							colliders[i]->OnTriggerEnter(trigger);
							colliders[j]->OnTriggerEnter(trigger);
							triggersEnteredPair.emplace(colliders[i], colliders[j]);
						}
					}
					else if (!colliders[i]->isTrigger)
					{
						float intersectionWidth = intersection.width;
						float intersectionHeight = intersection.height;
						Vector2Df intersectionPosition = { intersection.left - 0.5f * intersectionWidth, intersection.top - 0.5f * intersectionHeight };
						Vector2Df aPosition = { colliders[i]->bounds.left,  colliders[i]->bounds.top };
						auto aTransform = colliders[i]->GetGameObject()->GetComponent<TransformComponent>();

						if (intersectionWidth > intersectionHeight)
						{
							if (intersectionPosition.y > aPosition.y) aTransform->MoveBy({ 0, -intersectionHeight });
							else aTransform->MoveBy({ 0, intersectionHeight });
						}
						else
						{
							if (intersectionPosition.x > aPosition.x) aTransform->MoveBy({ -intersectionWidth, 0.f });
							else aTransform->MoveBy({ intersectionWidth, 0.f });
						}

						Collision collision(colliders[i], colliders[j], intersection);
						colliders[i]->OnCollision(collision);
						colliders[j]->OnCollision(collision);
					}
				} 
			}
		} 

	
		for (auto triggeredPair = triggersEnteredPair.cbegin(), nextTriggeredPair = triggeredPair; triggeredPair != triggersEnteredPair.cend(); triggeredPair = nextTriggeredPair)
		{
			++nextTriggeredPair;
			if (!triggeredPair->first->bounds.intersects(triggeredPair->second->bounds))
			{
				Trigger trigger(triggeredPair->first, triggeredPair->second);
				triggeredPair->first->OnTriggerExit(trigger);
				triggeredPair->second->OnTriggerExit(trigger);
				triggersEnteredPair.erase(triggeredPair);
			}
		}
	}


	void PhysicsSystem::Subscribe(ColliderComponent* collider)
	{
		colliders.push_back(collider);
	}
	void PhysicsSystem::Unsubscribe(ColliderComponent* collider)
	{
		colliders.erase(std::remove_if(colliders.begin(), colliders.end(), [collider](ColliderComponent* obj) { return obj == collider; }), colliders.end());
	}
}