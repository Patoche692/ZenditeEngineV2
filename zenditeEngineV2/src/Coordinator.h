#pragma once
#include "ECS/ECSCoordinator.h"
#include "ECS/Systems/RenderableSystem.h"

class Coordinator
{
private:
	std::shared_ptr<ECSCoordinator> m_ECSCoord;
	std::shared_ptr<RenderableSystem> m_RenderableSystem;

public:
	void RegisterComponents();

	void RegisterSystems();

	void SetUpSystems();

	Entity CreateEntity();

	template<typename T>
	void AddComponentToEntity(Entity EID, T comp)
	{
		m_ECSCoord->AddComponentToEntity<T>(EID, comp);
	}

	std::shared_ptr<ComponentManager> GetComponentManager()
	{
		return m_ECSCoord->GetComponentManager();
	}

};