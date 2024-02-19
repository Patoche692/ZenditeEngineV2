#pragma once
#include "ECS/ECSCoordinator.h"
#include "ECS/Systems/Im_CollisionDetectionSystem.h"
#include "ECS/Systems/Rigid_CollisionDetectionSystem.h"
#include "ECS/Systems/RenderableSystem.h"
#include "ECS/Systems/Trigger_CollisionDetectionSystem.h"
#include "API_Rendering/OpenGL_Manager.h"

class Camera;
class I_Renderer;

class Coordinator
{
private:
	std::shared_ptr<ECSCoordinator> m_ECSCoord;
	std::shared_ptr<I_Subject> m_Subject;

	std::shared_ptr<I_API_Manager> m_APImanager;
	std::shared_ptr<I_Renderer> m_Renderer;

	//Systems:
	std::shared_ptr<RenderableSystem> m_RenderableSystem;
	std::shared_ptr<Rigid_CollisionDetectionSystem> m_Rigid_CollisionDetectionSystem;
	std::shared_ptr<Im_CollisionDetectionSystem> m_Im_CollisionDetectionSystem;
	std::shared_ptr<Trigger_CollisionDetectionSystem> m_Trigger_CollisionDetectionSystem;

	
public:
	Coordinator(std::string API_Type, std::string Render_Type, std::shared_ptr<Camera> camera);

	void RegisterComponents();

	void RegisterSystems();

	void SetUpSystemBitsets();

	template<typename T>
	Signature GetSystemBitset()
	{
		return m_ECSCoord->GetSystemBitsetSignature<T>();
	}

	void SetUpRenderData(Entity EID);

	Entity CreateEntity();

	void runAllSystems(float deltaTime);
	
	void setShaderForEntity(Entity EID, std::shared_ptr<Shader> shader);

	void StoreShaderInEntityDataHandle(Entity EID);

	template<typename T>
	T& GetComponentDataFromEntity(Entity EID)
	{
		return m_ECSCoord->GetComponentDataFromEntity<T>(EID);
	}

	template<typename T>
	void AddComponentToEntity(Entity EID, T comp)
	{
		m_ECSCoord->AddComponentToEntity<T>(EID, comp);
	}

	std::shared_ptr<ComponentManager> GetComponentManager()
	{
		return m_ECSCoord->GetComponentManager();
	}

	unsigned short int GenerateTexUnit(std::string texFilePath, std::string fileType);

};