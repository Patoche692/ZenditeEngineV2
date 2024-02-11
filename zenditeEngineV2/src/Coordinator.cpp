#include "Coordinator.h"
#include "ECS/Components.h"
#include "Camera.h"
#include "../API_Rendering/OpenGl_Renderer.h"

Coordinator::Coordinator(std::string API_Type, std::string Render_Type, std::shared_ptr<Camera> camera)
{
	m_ECSCoord = std::make_shared<ECSCoordinator>();

	if (API_Type == "opengl")
	{
		m_APImanager = std::make_shared<OpenGL_Manager>();
	}
	else
	{
		//default:
		m_APImanager = std::make_shared<OpenGL_Manager>();
	}

	if(Render_Type == "opengl")
	{		
		m_Renderer = std::make_shared<OpenGL_Renderer>(camera);
	}
	else
	{
		m_Renderer = std::make_shared<OpenGL_Renderer>(camera);
	}
}

void Coordinator::RegisterComponents()
{
	m_ECSCoord->RegisterComponent<c_Transform>();
	m_ECSCoord->RegisterComponent<c_RenderableComponent>();
	m_ECSCoord->RegisterComponent<c_RigidBodyCollidable>();
	m_ECSCoord->RegisterComponent<c_ImmovableCollidable>();
	m_ECSCoord->RegisterComponent<c_Modified>();
	m_ECSCoord->RegisterComponent<c_CollidableTrigger>();
}

void Coordinator::RegisterSystems() //And add them to the system manager list
{
	m_RenderableSystem = std::static_pointer_cast<RenderableSystem>(m_ECSCoord->RegisterSystem<RenderableSystem>());
	m_Rigid_CollisionDetectionSystem = std::static_pointer_cast<Rigid_CollisionDetectionSystem>(m_ECSCoord->RegisterSystem<Rigid_CollisionDetectionSystem>());
	m_Im_CollisionDetectionSystem = std::static_pointer_cast<Im_CollisionDetectionSystem>(m_ECSCoord->RegisterSystem<Im_CollisionDetectionSystem>());
	m_Trigger_CollisionDetectionSystem = std::static_pointer_cast<Trigger_CollisionDetectionSystem>(m_ECSCoord->RegisterSystem<Trigger_CollisionDetectionSystem>());
}

void Coordinator::SetUpSystemBitsets()
{

	Signature RenerableSysSig;
	RenerableSysSig.set(m_ECSCoord->GetComponentBitsetPos<c_Transform>());
	RenerableSysSig.set(m_ECSCoord->GetComponentBitsetPos<c_RenderableComponent>());
	RenerableSysSig.set(m_ECSCoord->GetComponentBitsetPos<c_Modified>());
	m_ECSCoord->SetSystemBitsetSignature<RenderableSystem>(RenerableSysSig);

	Signature Rigid_CollisionDetectionSystemSig;
	Rigid_CollisionDetectionSystemSig.set(m_ECSCoord->GetComponentBitsetPos<c_Transform>());
	Rigid_CollisionDetectionSystemSig.set(m_ECSCoord->GetComponentBitsetPos<c_RigidBodyCollidable>());
	Rigid_CollisionDetectionSystemSig.set(m_ECSCoord->GetComponentBitsetPos<c_Modified>());
	m_ECSCoord->SetSystemBitsetSignature<Rigid_CollisionDetectionSystem>(Rigid_CollisionDetectionSystemSig);

	Signature Im_CollisionDetectionSystemSig;
	Im_CollisionDetectionSystemSig.set(m_ECSCoord->GetComponentBitsetPos<c_Transform>());
	Im_CollisionDetectionSystemSig.set(m_ECSCoord->GetComponentBitsetPos<c_ImmovableCollidable>());
	Im_CollisionDetectionSystemSig.set(m_ECSCoord->GetComponentBitsetPos<c_Modified>());
	m_ECSCoord->SetSystemBitsetSignature<Im_CollisionDetectionSystem>(Im_CollisionDetectionSystemSig);

	Signature Trigger_CollisionDetectionSystemSig;
	Trigger_CollisionDetectionSystemSig.set(m_ECSCoord->GetComponentBitsetPos<c_Transform>());
	Trigger_CollisionDetectionSystemSig.set(m_ECSCoord->GetComponentBitsetPos<c_RigidBodyCollidable>());
	Trigger_CollisionDetectionSystemSig.set(m_ECSCoord->GetComponentBitsetPos<c_Modified>());
	m_ECSCoord->SetSystemBitsetSignature<Trigger_CollisionDetectionSystem>(Trigger_CollisionDetectionSystemSig);

}

void Coordinator::SetUpRenderData(Entity EID)
{
	m_APImanager->SetupRenderData(EID, m_ECSCoord);
}

Entity Coordinator::CreateEntity()
{
	return m_ECSCoord->CreateEntity();
}

void Coordinator::setShaderForEntity(Entity EID, std::shared_ptr<Shader> shader)
{
	m_APImanager->SetShaderForEntity(EID, shader);
}

void Coordinator::runAllSystems(float deltaTime)
{
	m_RenderableSystem->Render(m_Renderer, m_APImanager, m_ECSCoord);
}


