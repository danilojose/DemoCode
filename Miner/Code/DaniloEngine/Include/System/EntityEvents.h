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

	///////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// Player Collision Detected
	/// </summary>
	struct EvtData_SwapEntitiesRequested : public BaseEventData
	{

		static const EventType sk_EventType;
		virtual const EventType & GetEventType(void) const
		{
			return sk_EventType;
		}

		explicit EvtData_SwapEntitiesRequested(const uint32_t &entityIdOne, const uint32_t &entityIdTwo )
			: m_EntityIdOne(entityIdOne), m_EntityIdTwo(entityIdTwo)
		{}


		virtual IEventDataPtr VCopy() const
		{
			return IEventDataPtr(GCC_NEW EvtData_SwapEntitiesRequested(m_EntityIdOne,m_EntityIdTwo));
		}

		virtual ~EvtData_SwapEntitiesRequested()
		{
		}

		uint32_t m_EntityIdOne;
		uint32_t m_EntityIdTwo;
	};

	///////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// Player Collision Detected
	/// </summary>
	struct EvtData_StoneMovementRequested : public BaseEventData
	{

		static const EventType sk_EventType;
		virtual const EventType & GetEventType(void) const
		{
			return sk_EventType;
		}

		explicit EvtData_StoneMovementRequested(const std::pair<uint16_t,uint16_t> &targetPosition, const std::pair<uint16_t,uint16_t> &middlePosition)
			: m_TargetPosition(targetPosition), m_MiddlePosition(middlePosition)
		{}


		virtual IEventDataPtr VCopy() const
		{
			return IEventDataPtr(GCC_NEW EvtData_StoneMovementRequested(m_TargetPosition, m_MiddlePosition));
		}

		virtual ~EvtData_StoneMovementRequested()
		{
		}

		uint32_t m_EntityId;
		std::pair<uint16_t, uint16_t> m_TargetPosition;
		std::pair<uint16_t, uint16_t> m_MiddlePosition;
	};
}

