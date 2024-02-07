#include "Coordinator.h"
#include "ECS/Components.h"


void Coordinator::RegisterComponents()
{
	m_ECSCoord->RegisterComponent<c_Transform>();
	m_ECSCoord->RegisterComponent<c_RenderableComponent>();
	m_ECSCoord->RegisterComponent<c_RigidBody>();
	m_ECSCoord->RegisterComponent<c_ImmovableCollidable>();
	m_ECSCoord->RegisterComponent<c_Modified>();
}

void Coordinator::RegisterSystems()
{
	m_RenderableSystem = std::static_pointer_cast<RenderableSystem>(m_ECSCoord->RegisterSystem<RenderableSystem>());

}

void Coordinator::SetUpSystems()
{

}

Entity Coordinator::CreateEntity()
{

}
