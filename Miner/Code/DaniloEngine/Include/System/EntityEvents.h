#pragma once
#include <System\Events.h>

namespace GameSystem
{
	///////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// Player Collision Detected
	/// </summary>
	struct EvtData_OnEntitySelected : public BaseEventData
	{

		static const EventType sk_EventType;
		virtual const EventType & GetEventType(void) const
		{
			return sk_EventType;
		}

		explicit EvtData_OnEntitySelected(const uint32_t &entityId)
			: m_EntityId(entityId)
		{}


		virtual IEventDataPtr VCopy() const
		{
			return IEventDataPtr(GCC_NEW EvtData_OnEntitySelected(m_EntityId));
		}

		virtual ~EvtData_OnEntitySelected()
		{
		}

		uint32_t m_EntityId;
	};

	///////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// Player Collision Detected
	/// </summary>
	struct EvtData_OnEntityUnSelected : public BaseEventData
	{

		static const EventType sk_EventType;
		virtual const EventType & GetEventType(void) const
		{
			return sk_EventType;
		}

		explicit EvtData_OnEntityUnSelected(const uint32_t &entityId)
			: m_EntityId(entityId)
		{}


		virtual IEventDataPtr VCopy() const
		{
			return IEventDataPtr(GCC_NEW EvtData_OnEntityUnSelected(m_EntityId));
		}

		virtual ~EvtData_OnEntityUnSelected()
		{
		}

		uint32_t m_EntityId;
	};

}

