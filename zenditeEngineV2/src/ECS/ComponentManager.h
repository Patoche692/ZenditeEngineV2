#pragma once
#include "ComponentArray.h"

class ComponentManager
{
private:
	std::unordered_map<const char*, std::shared_ptr<I_ComponentArray>> m_map_COMPTAGtoCOMPARRAY;

	std::unordered_map<const char*, ComponentBitsetPos> m_map_COMPTAGtoBITSETPOS;

	ComponentBitsetPos m_NextAvailableBitsetPosition = 0;

public:

	template<typename T>
	void RegisterComponent()
	{
		const char* tag = typeid(T).name(); //#FUTURE_FIX_Change_tag_to_a_std_string_as_const_char_ptr_is_not_portable_across_all

		DEBUG_ASSERT(m_map_COMPTAGtoCOMPARRAY.find(tag) == m_map_COMPTAGtoCOMPARRAY.end(), "Attempting to register the same component more than once is prohibited");
		
		m_map_COMPTAGtoBITSETPOS[tag] = m_NextAvailableBitsetPosition;
		++m_NextAvailableBitsetPosition;

		m_map_COMPTAGtoCOMPARRAY[tag] = std::make_shared<ComponentArray<T>>();
	}

	void EntityDestroyed(Entity EID)
	{
		for(const std::pair<const char* const, std::shared_ptr<I_ComponentArray>>& pair : m_map_COMPTAGtoCOMPARRAY)
		{
			std::shared_ptr<I_ComponentArray> component = pair.second;
			component->EntityDestroyed(EID);
		}
	}

	template <typename T>
	void AddComponentToEntity(Entity EID, T componentData)
	{
		GetComponentArray<T>()->InsertData(EID, componentData);
	}

	template<typename T>
	void RemoveComponentFromEntity(Entity EID)
	{
		const char* tag = typeid(T).name();
		GetComponentArray<T>()->RemoveComponentDataFromRelatedEntity(EID);
	}

	template<typename T>
	T& GetSpecificComponentData(Entity EID)
	{
		return GetComponentArray<T>()->GetSpecificComponentData(EID);
	}

	template<typename T>
	std::shared_ptr<ComponentArray<T>> GetComponentArray()
	{
		const char* tag = typeid(T).name();

		DEBUG_ASSERT(m_map_COMPTAGtoCOMPARRAY.find(tag) != m_map_COMPTAGtoCOMPARRAY.end(), "Component Array does not exist. You need to register the component first");

		return std::static_pointer_cast<ComponentArray<T>>(m_map_COMPTAGtoCOMPARRAY[tag]); //Need to static cast as the component array is of type I_ComponentArray not component array.
	}

	template<typename T>
	ComponentBitsetPos GetComponentBitsetPos() const
	{
		const char* tag = typeid(T).name();

		DEBUG_ASSERT(m_map_COMPTAGtoBITSETPOS.find(tag) != m_map_COMPTAGtoBITSETPOS.end(), "Bitset position of Requested Component does not exist. Please register the component first.");

		return m_map_COMPTAGtoBITSETPOS[tag];
	}

};