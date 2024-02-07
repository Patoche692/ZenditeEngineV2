#pragma once

#include "I_System.h"

class SystemManager
{
private:
	std::unordered_map<const char*, Signature> m_map_SYSTAGtoSIGNATURE;
	std::unordered_map<const char*, std::shared_ptr<I_System>> m_map_SYSTAGtoISYSTEM;

public:
	template <typename T>
	std::shared_ptr<T> GenerateAndRegisterSystem()
	{
		const char* systag = typeid(T).name();

		DEBUG_ASSERT(m_map_SYSTAGtoISYSTEM.find(systag) == m_map_SYSTAGtoISYSTEM.end(), "Attemting to register the same system more than once is prohibited");

		m_map_SYSTAGtoISYSTEM[systag] = std::make_shared<T>();
	}

	template<typename T>
	void SetSystemSignature(Signature sig)
	{
		const char* systag = typeid(T).name();
		DEBUG_ASSERT(m_map_SYSTAGtoISYSTEM.find(systag) != m_map_SYSTAGtoISYSTEM.end(), "System needs to be registered before a signature can be added");

		m_map_SYSTAGtoSIGNATURE[systag] = sig;
	}

	//When an entity is destroyed its EID needs to removed from all systems std::set<Entity> m_EntitySet; which is inherited from I_System
	void EntityDestroyed(Entity EID)
	{
		for (const std::pair<const char* const, std::shared_ptr<I_System>> const& pair : m_map_SYSTAGtoISYSTEM)
		{
			std::shared_ptr<I_System> const& system = pair.second;

			system->m_EntitySet.erase(EID);
		}
	}

	//This is the function that is called every time a component is added or removed from an entity. When this happens we notify every system and check if the signature is now valid or invalid and the add or remove it accordingly
	void EntitySignatureChanged(Entity EID, Signature entitySig)
	{
		for (const std::pair<const char* const, std::shared_ptr<I_System>> const& it : m_map_SYSTAGtoISYSTEM)
		{
			auto const& sysTag = it.first;
			auto const& system = it.second;
			auto const& sysSig = m_map_SYSTAGtoSIGNATURE[sysTag];

			//Note: remember an entities signature is determined by the components it has attached to it. It can be retrieved using

			if ((entitySig & sysSig) == sysSig) //this is a Bit-wise AND it compares if there are at least as many ones in the same place in entitySignature as there are in sysSig
			{
				system->m_EntitySet.insert(EID); //If it matches then this entity becomes part of the system.
			}
			else
			{
				system->m_EntitySet.erase(EID); //If not, then it should not be part of this system and is, as such, removed.
			}
		}
	}

	template<typename T>
	std::bitset<MAX_COMPONENTS> GetSystemBitset()
	{
		const char* sysTag = typeid(T).name();

		return m_map_SYSTAGtoSIGNATURE[sysTag];

	}

};