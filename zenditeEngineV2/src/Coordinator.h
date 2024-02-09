#pragma once
#include "ECS/ECSCoordinator.h"
#include "ECS/Systems/Im_CollisionDetectionSystem.h"
#include "ECS/Systems/Rigid_CollisionDetectionSystem.h"
#include "ECS/Systems/RenderableSystem.h"
#include "ECS/Systems/Trigger_CollisionDetectionSystem.h"
#include "API_Rendering/OpenGL_Manager.h"

class Coordinator
{
private:
	std::shared_ptr<ECSCoordinator> m_ECSCoord;
	std::shared_ptr<I_Subject> m_Subject;

	std::unique_ptr<I_API_Manager> m_APImanager;

	//Systems:
	std::shared_ptr<RenderableSystem> m_RenderableSystem;
	std::shared_ptr<Rigid_CollisionDetectionSystem> m_Rigid_CollisionDetectionSystem;
	std::shared_ptr<Im_CollisionDetectionSystem> m_Im_CollisionDetectionSystem;
	std::shared_ptr<Trigger_CollisionDetectionSystem> m_Trigger_CollisionDetectionSystem;

	
public:
	Coordinator(std::string API_Type);

	void RegisterComponents();

	void RegisterSystems();

	void SetUpSystemBitsets();

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