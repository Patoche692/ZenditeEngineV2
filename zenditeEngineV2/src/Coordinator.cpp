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
	m_ECSCoord->RegisterComponent<c_Renderable>();
	m_ECSCoord->RegisterComponent<c_Texture>();
	m_ECSCoord->RegisterComponent<c_AABB>();
	m_ECSCoord->RegisterComponent<c_Modified>();
	m_ECSCoord->RegisterComponent<c_PointLightEmitter>();
	m_ECSCoord->RegisterComponent<c_SpotLightEmitter>();
	m_ECSCoord->RegisterComponent<c_DirLightEmitter>();

	m_ECSCoord->RegisterComponent<c_LightRenderable>();

	m_ECSCoord->RegisterComponent<c_Wall>();
	m_ECSCoord->RegisterComponent<c_WallCollider>();
	m_ECSCoord->RegisterComponent<c_EntityInfo>();


}

void Coordinator::RegisterSystems() //And add them to the system manager list
{
	m_RenderableSystem = std::static_pointer_cast<RenderableSystem>(m_ECSCoord->RegisterSystem<RenderableSystem>());

	m_SetupPointLightSystem = std::static_pointer_cast<SetupPointLightSystem>(m_ECSCoord->RegisterSystem<SetupPointLightSystem>());
	m_SetupSpotLightSystem = std::static_pointer_cast<SetupSpotLightSystem>(m_ECSCoord->RegisterSystem<SetupSpotLightSystem>());
	m_SetupDirLightSystem = std::static_pointer_cast<SetupDirLightSystem>(m_ECSCoord->RegisterSystem<SetupDirLightSystem>());
	m_CollisionDetectionAABBSystem = std::static_pointer_cast<CollisionDetectionAABBSystem>(m_ECSCoord->RegisterSystem<CollisionDetectionAABBSystem>());
	m_RenderAABBSystem = std::static_pointer_cast<RenderAABBSystem>(m_ECSCoord->RegisterSystem<RenderAABBSystem>());
	m_SetUpWallAABBSystem = std::static_pointer_cast<SetUpWallAABBSystem>(m_ECSCoord->RegisterSystem<SetUpWallAABBSystem>());
	m_SetUpWallColliderAABBSystem = std::static_pointer_cast<SetUpWallColliderAABBSystem>(m_ECSCoord->RegisterSystem<SetUpWallColliderAABBSystem>());
	m_WallCollisionHandlingSystem = std::static_pointer_cast<WallCollisionHandlingSystem>(m_ECSCoord->RegisterSystem<WallCollisionHandlingSystem>());
	m_PositionTrackerSystem = std::static_pointer_cast<PositionTrackerSystem>(m_ECSCoord->RegisterSystem<PositionTrackerSystem>());

	m_RenderDirLightSourceSystem = std::static_pointer_cast<RenderDirLightSourceSystem>(m_ECSCoord->RegisterSystem<RenderDirLightSourceSystem>());
	m_RenderPointLightSourceSystem = std::static_pointer_cast<RenderPointLightSourceSystem>(m_ECSCoord->RegisterSystem<RenderPointLightSourceSystem>());
	m_RenderSpotLightSourceSystem = std::static_pointer_cast<RenderSpotLightSourceSystem>(m_ECSCoord->RegisterSystem<RenderSpotLightSourceSystem>());
	
	//Extra: (Prevent garbage memory being accessed if there are no entities will wall related AABB components)
	m_SetUpWallAABBSystem->InitialSetup(m_ECSCoord); 
	m_SetUpWallColliderAABBSystem->InitialSetup(m_ECSCoord);
}

void Coordinator::SetUpSystemBitsets()
{
	Signature RenerableSysSig;
	RenerableSysSig.set(m_ECSCoord->GetComponentBitsetPos<c_Transform>());
	RenerableSysSig.set(m_ECSCoord->GetComponentBitsetPos<c_Renderable>());
	RenerableSysSig.set(m_ECSCoord->GetComponentBitsetPos<c_Modified>());
	m_ECSCoord->SetSystemBitsetSignature<RenderableSystem>(RenerableSysSig);

	Signature PositionTrackerSystemSig;
	PositionTrackerSystemSig.set(m_ECSCoord->GetComponentBitsetPos<c_Transform>());
	m_ECSCoord->SetSystemBitsetSignature<PositionTrackerSystem>(PositionTrackerSystemSig);

	Signature CollisionDetectionAABBSystemSig;
	CollisionDetectionAABBSystemSig.set(m_ECSCoord->GetComponentBitsetPos<c_Transform>());
	CollisionDetectionAABBSystemSig.set(m_ECSCoord->GetComponentBitsetPos<c_AABB>());
	CollisionDetectionAABBSystemSig.set(m_ECSCoord->GetComponentBitsetPos<c_Modified>());
	m_ECSCoord->SetSystemBitsetSignature<CollisionDetectionAABBSystem>(CollisionDetectionAABBSystemSig);

	Signature RenderAABBSystemSig;
	RenderAABBSystemSig.set(m_ECSCoord->GetComponentBitsetPos<c_Transform>());
	RenderAABBSystemSig.set(m_ECSCoord->GetComponentBitsetPos<c_AABB>());
	RenderAABBSystemSig.set(m_ECSCoord->GetComponentBitsetPos<c_Modified>());
	m_ECSCoord->SetSystemBitsetSignature<RenderAABBSystem>(RenderAABBSystemSig);

	Signature SetupPointLightSystemSig;
	SetupPointLightSystemSig.set(m_ECSCoord->GetComponentBitsetPos<c_Transform>());
	SetupPointLightSystemSig.set(m_ECSCoord->GetComponentBitsetPos<c_PointLightEmitter>());
	SetupPointLightSystemSig.set(m_ECSCoord->GetComponentBitsetPos<c_Modified>());
	m_ECSCoord->SetSystemBitsetSignature<SetupPointLightSystem>(SetupPointLightSystemSig);

	Signature SetupSpotLightSystemSig;
	SetupSpotLightSystemSig.set(m_ECSCoord->GetComponentBitsetPos<c_Transform>());
	SetupSpotLightSystemSig.set(m_ECSCoord->GetComponentBitsetPos<c_SpotLightEmitter>());
	SetupSpotLightSystemSig.set(m_ECSCoord->GetComponentBitsetPos<c_Modified>());
	m_ECSCoord->SetSystemBitsetSignature<SetupSpotLightSystem>(SetupSpotLightSystemSig);

	Signature SetupDirLightSystemSig;
	SetupDirLightSystemSig.set(m_ECSCoord->GetComponentBitsetPos<c_Transform>());
	SetupDirLightSystemSig.set(m_ECSCoord->GetComponentBitsetPos<c_DirLightEmitter>());
	SetupDirLightSystemSig.set(m_ECSCoord->GetComponentBitsetPos<c_Modified>());
	m_ECSCoord->SetSystemBitsetSignature<SetupDirLightSystem>(SetupDirLightSystemSig);

	//m_RenderDirLightSourceSystem
	//m_RenderPointLightSourceSystem
	//m_RenderSpotLightSourceSystem

	Signature RenderDirLightSourceSystemSig;
	RenderDirLightSourceSystemSig.set(m_ECSCoord->GetComponentBitsetPos<c_Transform>());
	RenderDirLightSourceSystemSig.set(m_ECSCoord->GetComponentBitsetPos<c_LightRenderable>());
	RenderDirLightSourceSystemSig.set(m_ECSCoord->GetComponentBitsetPos<c_DirLightEmitter>());
	RenderDirLightSourceSystemSig.set(m_ECSCoord->GetComponentBitsetPos<c_Modified>());
	m_ECSCoord->SetSystemBitsetSignature<RenderDirLightSourceSystem>(RenderDirLightSourceSystemSig);

	Signature RenderPointLightSourceSystemSig;
	RenderPointLightSourceSystemSig.set(m_ECSCoord->GetComponentBitsetPos<c_Transform>());
	RenderPointLightSourceSystemSig.set(m_ECSCoord->GetComponentBitsetPos<c_LightRenderable>());
	RenderPointLightSourceSystemSig.set(m_ECSCoord->GetComponentBitsetPos<c_PointLightEmitter>());
	RenderPointLightSourceSystemSig.set(m_ECSCoord->GetComponentBitsetPos<c_Modified>());
	m_ECSCoord->SetSystemBitsetSignature<RenderPointLightSourceSystem>(RenderPointLightSourceSystemSig);

	Signature RenderSpotLightSourceSystemSig;
	RenderSpotLightSourceSystemSig.set(m_ECSCoord->GetComponentBitsetPos<c_Transform>());
	RenderSpotLightSourceSystemSig.set(m_ECSCoord->GetComponentBitsetPos<c_LightRenderable>());
	RenderSpotLightSourceSystemSig.set(m_ECSCoord->GetComponentBitsetPos<c_SpotLightEmitter>());
	RenderSpotLightSourceSystemSig.set(m_ECSCoord->GetComponentBitsetPos<c_Modified>());
	m_ECSCoord->SetSystemBitsetSignature<RenderSpotLightSourceSystem>(RenderSpotLightSourceSystemSig);

	Signature SetUpWallAABBSystemSig;
	SetUpWallAABBSystemSig.set(m_ECSCoord->GetComponentBitsetPos<c_Transform>());
	SetUpWallAABBSystemSig.set(m_ECSCoord->GetComponentBitsetPos<c_AABB>());
	SetUpWallAABBSystemSig.set(m_ECSCoord->GetComponentBitsetPos<c_Wall>());
	m_ECSCoord->SetSystemBitsetSignature<SetUpWallAABBSystem>(SetUpWallAABBSystemSig);

	Signature SetUpWallColliderAABBSystemSig;
	SetUpWallColliderAABBSystemSig.set(m_ECSCoord->GetComponentBitsetPos<c_Transform>());
	SetUpWallColliderAABBSystemSig.set(m_ECSCoord->GetComponentBitsetPos<c_AABB>());
	SetUpWallColliderAABBSystemSig.set(m_ECSCoord->GetComponentBitsetPos<c_WallCollider>());
	m_ECSCoord->SetSystemBitsetSignature<SetUpWallColliderAABBSystem>(SetUpWallColliderAABBSystemSig);

	Signature WallCollisionHandlingSystemSig;
	//Entities are assigned differently for this one, as such no sig is needed as of yet.
	m_ECSCoord->SetSystemBitsetSignature<WallCollisionHandlingSystem>(WallCollisionHandlingSystemSig);
}

Signature Coordinator::GetEntitySignature(Entity EID)
{
	return m_ECSCoord->GetEntitySignature(EID);
}

void Coordinator::SetUpRenderData(Entity EID)
{
	// if ((entitySig & sysSig) == sysSig)

	if ((m_ECSCoord->GetEntitySignature(EID) & m_ECSCoord->GetSystemBitsetSignature<RenderableSystem>()) == m_ECSCoord->GetSystemBitsetSignature<RenderableSystem>())
	{
		m_APImanager->SetupRenderData(EID, m_ECSCoord);
	}
	else
	{
		std::cout << "\nNOTIFY: Attempting to Set Up Render Data on EID: " << EID << " Which is without a renderable component" << std::endl;
	}
}

Entity Coordinator::CreateEntity()
{
	return m_ECSCoord->CreateEntity();
}

void Coordinator::setShaderForEntity(Entity EID, std::shared_ptr<Shader> shader)
{
	if ((m_ECSCoord->GetEntitySignature(EID) & m_ECSCoord->GetSystemBitsetSignature<RenderableSystem>()) == m_ECSCoord->GetSystemBitsetSignature<RenderableSystem>())
	{
		m_APImanager->SetShaderForEntity(EID, shader);
	}
	else
	{
		std::cout << "NOTIFY: Attempting to set shader for EID: " << EID << " which is without a renderable component" << std::endl;
	}
}

void Coordinator::StoreShaderInEntityDataHandle(Entity EID)
{
	if ((m_ECSCoord->GetEntitySignature(EID) & m_ECSCoord->GetSystemBitsetSignature<RenderableSystem>()) == m_ECSCoord->GetSystemBitsetSignature<RenderableSystem>())
	{
		m_APImanager->SetShaderForDataHandle(EID);
	}
	else
	{
		std::cout << "NOTIFY: Attempting to set shader for EID: " << EID << " which is without a renderable component" << std::endl;
	}
}

unsigned short int Coordinator::GenerateTexUnit(std::string texFilePath, std::string fileType)
{
	return m_APImanager->GenerateTexUnit(texFilePath, fileType);
}

void Coordinator::GenerateShadowMapForEntity(Entity EID) {
	c_DirLightEmitter& DirLightData = m_ECSCoord->GetComponentDataFromEntity<c_DirLightEmitter>(EID);
	m_APImanager->GenerateDepthMap(DirLightData.depthMapFBO, DirLightData.depthMapUnit);
}

uint32_t Coordinator::GetActiveEntities() const
{
	return m_ECSCoord->GetActiveEntities();
}

void Coordinator::runAllSystems(float deltaTime, std::vector<Entity>* entities)
{
	m_SetupPointLightSystem->Setup(m_APImanager, m_ECSCoord);
	m_SetupSpotLightSystem->Setup(m_APImanager, m_ECSCoord);
	m_SetupDirLightSystem->Setup(m_APImanager, m_ECSCoord);
	m_RenderableSystem->Render(m_Renderer, m_APImanager, m_ECSCoord);
	m_CollisionDetectionAABBSystem->checkCollisions(m_ECSCoord);
	
	m_SetUpWallAABBSystem->Setup(m_ECSCoord);
	m_SetUpWallColliderAABBSystem->Setup(m_ECSCoord);
	m_WallCollisionHandlingSystem->checkCollisions(m_ECSCoord);
	
	m_RenderAABBSystem->RenderAABBs(m_Renderer, m_APImanager, m_ECSCoord);

	m_RenderPointLightSourceSystem->Render(m_Renderer, m_APImanager, m_ECSCoord);
	m_RenderSpotLightSourceSystem->Render(m_Renderer, m_APImanager, m_ECSCoord);
	m_RenderDirLightSourceSystem->Render(m_Renderer, m_APImanager, m_ECSCoord);

	m_PositionTrackerSystem->UpdatePrePosData(m_ECSCoord);

	for (auto const& EID : *entities)
	{
		m_ECSCoord->GetComponentDataFromEntity<c_Modified>(EID).isModifed = false;
	}
	
}


