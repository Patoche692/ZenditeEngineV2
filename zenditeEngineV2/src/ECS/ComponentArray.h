#pragma once
#include "ECSUtils.h"

class I_ComponentArray
{
public:
	virtual ~I_ComponentArray() = default;
	virtual void EntityDestroyed(Entity EID) = 0;
};

template<typename T>
class ComponentArray : public I_ComponentArray
{
private:
	std::array<T, MAX_ENTITIES> m_ComponentArray = {};

	std::unordered_map<Entity, size_t> m_map_EntityToIndex;
	std::unordered_map<size_t, Entity> m_map_IndexToEntity;

	size_t m_arraySize = 0;

public:
	void EntityDestroyed(Entity EID_TD) override
	{
		if (m_map_EntityToIndex.find(EID_TD) != m_map_EntityToIndex.end())
		{
			RemoveComponentDataFromRelatedEntity(EID_TD);
		}

	}

	void InsertData(Entity EID, T component)
	{
		m_ComponentArray[m_arraySize] = component;

		m_map_EntityToIndex[EID] = m_arraySize;
		m_map_IndexToEntity[m_arraySize] = EID;

		++m_arraySize;

	}

	//itteration 68?
	T& GetSpecificComponentData(Entity EID)
	{
		DEBUG_ASSERT(m_map_EntityToIndex.find(EID) != m_map_EntityToIndex.end(), "Entity requested does not contain component data of this type.");
		size_t index = m_map_EntityToIndex[EID];
		return m_ComponentArray[index];
	}

	std::array<T, MAX_ENTITIES> const& GetComponentArray() const
	{
		return m_ComponentArray;
	}

	//This member function copies the final index into the component slot to be deleted and then adjusts the maps to reflect this change.
	//Doing this keeps the component array data tightly packed which allows for better cache spatial locality, leading to improved overall performance (hopefully)
	void RemoveComponentDataFromRelatedEntity(Entity EID_TD)
	{
		DEBUG_ASSERT(m_map_EntityToIndex.find(EID_TD) != m_map_EntityToIndex.end(), "Attempting to remove component data from an entity that does not contain the relevant component data");
	
		size_t EID_TD_Index = m_map_EntityToIndex[EID_TD];
		size_t EID_TA_Index = m_arraySize - 1;
		Entity EID_TA = m_map_IndexToEntity[EID_TA_Index];
	
		m_ComponentArray[EID_TD_Index] = m_ComponentArray[EID_TA_Index]; //Copy final index component into deleted entities component slot

		//Update the maps to reflect this change:
		m_map_EntityToIndex[EID_TA] = EID_TD_Index;
		m_map_IndexToEntity[EID_TD_Index] = EID_TA;

		m_map_EntityToIndex.erase(EID_TD);
		m_map_IndexToEntity.erase(EID_TA_Index);

		--m_arraySize; //Shift final index down by one
	}

};