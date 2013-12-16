#pragma once
#include <System\StdLibraries.h>
#include <System\HashedString.h>
#include <strstream>


namespace GameSystem
{

//typedefs and forward declarations
class IEventData;
typedef std::shared_ptr<IEventData> IEventDataPtr;
typedef HashedString EventType;


/// <summary>
/// The basic Event Data class that provides implementation for the below 
/// </summary>
class IEventData
{
public:
	virtual const EventType & GetEventType(void) const = 0;
	virtual float GetTimeStamp() const = 0;

	virtual IEventDataPtr VCopy() const = 0;
};


/// <summary>
/// BaseEventData used by every other EventData
/// </summary>
struct BaseEventData : public IEventData
{
public:
	BaseEventData(const float timeStamp = 0.0f)
		: m_TimeStamp(timeStamp)
	{
	}
	virtual ~BaseEventData()	{ }

	//Returns the type of the event
	virtual const EventType & GetEventType(void) const = 0;

	float GetTimeStamp(void) const
	{
		return m_TimeStamp;
	}

protected:
	const float m_TimeStamp;

private : 
	
	BaseEventData& operator=(const BaseEventData&) = delete;

};


/// <summary>
/// Helper for events that require no data
/// </summary>
struct EmptyEventData : public BaseEventData
{
public:
	explicit EmptyEventData(const float timeStamp = 0.0f)
		: BaseEventData(timeStamp)
	{
	}
	virtual ~EmptyEventData()	{ }


private:
	EmptyEventData& operator=(const EmptyEventData&) = delete;
};

/// <summary>
/// Templated Event for templated data 
/// </summary>
template < typename T >
struct EvtData : public BaseEventData

{
private:
	EvtData();					// disable default construction
	EvtData(const EvtData &);	// disable copy construction
	EvtData& operator=(const EvtData&) = delete;
	T m_Value;

public:
	explicit EvtData<T>(T n,const float timestamp=0)
		: BaseEventData(timestamp)
	{
		m_Value = n;
	}

	const T GetValue() { return m_Value; }

};

///////////////////////////////////////////////////////////////////////////////
/// <summary>
/// Update Position Event
/// </summary>
struct EvtData_UpdatePosition : public BaseEventData
{

	static const EventType sk_EventType;
	virtual const EventType & GetEventType(void) const
	{
		return sk_EventType;
	}

	explicit EvtData_UpdatePosition(uint32_t id, uint32_t posX, uint32_t posY)
		: m_ActorId(id),
		m_PosX(posX),
		m_PosY(posY)
	{}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr(GCC_NEW EvtData_UpdatePosition(m_ActorId, m_PosX,m_PosY));
	}

	virtual ~EvtData_UpdatePosition()
	{
	}

	uint32_t m_ActorId;
	uint32_t m_PosX;
	uint32_t m_PosY;


};

///////////////////////////////////////////////////////////////////////////////
/// <summary>
/// Create player fire event
/// </summary>
struct EvtData_CreatePlayerFire : public BaseEventData
{

	static const EventType sk_EventType;
	virtual const EventType & GetEventType(void) const
	{
		return sk_EventType;
	}

	explicit EvtData_CreatePlayerFire(uint32_t id,uint32_t posX, uint32_t posY)
		: m_id(id),
		m_PosX(posX),
		m_PosY(posY)
	{}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr(GCC_NEW EvtData_CreatePlayerFire(m_id, m_PosX, m_PosY));
	}

	virtual ~EvtData_CreatePlayerFire()
	{
	}

	uint32_t m_id;
	uint32_t m_PosX;
	uint32_t m_PosY;


};

///////////////////////////////////////////////////////////////////////////////
/// <summary>
/// Create Enemy Fire event
/// </summary>
struct EvtData_CreateEnemyFire : public BaseEventData
{

	static const EventType sk_EventType;
	virtual const EventType & GetEventType(void) const
	{
		return sk_EventType;
	}

	explicit EvtData_CreateEnemyFire(uint32_t id, uint32_t posX, uint32_t posY)
		: m_id(id),
		m_PosX(posX),
		m_PosY(posY)
	{}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr(GCC_NEW EvtData_CreateEnemyFire(m_id, m_PosX, m_PosY));
	}

	virtual ~EvtData_CreateEnemyFire()
	{
	}

	uint32_t m_id;
	uint32_t m_PosX;
	uint32_t m_PosY;


};

///////////////////////////////////////////////////////////////////////////////
/// <summary>
/// Create Enemy Explosion event
/// </summary>
struct EvtData_CreateEnemyExplosion : public BaseEventData
{

	static const EventType sk_EventType;
	virtual const EventType & GetEventType(void) const
	{
		return sk_EventType;
	}

	explicit EvtData_CreateEnemyExplosion(uint32_t parentid, uint32_t posX, uint32_t posY)
		: m_id(parentid),
		m_PosX(posX),
		m_PosY(posY)

	{}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr(GCC_NEW EvtData_CreateEnemyExplosion(m_id, m_PosX, m_PosY));
	}

	virtual ~EvtData_CreateEnemyExplosion()
	{
	}

	uint32_t m_id;
	uint32_t m_PosX;
	uint32_t m_PosY;

};
///////////////////////////////////////////////////////////////////////////////
/// <summary>
/// Create Player Explosion event
/// </summary>
struct EvtData_CreatePlayerExplosion : public BaseEventData
{

	static const EventType sk_EventType;
	virtual const EventType & GetEventType(void) const
	{
		return sk_EventType;
	}

	explicit EvtData_CreatePlayerExplosion(uint32_t parentid, uint32_t posX, uint32_t posY)
		: m_id(parentid),
		m_PosX(posX),
		m_PosY(posY)

	{}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr(GCC_NEW EvtData_CreatePlayerExplosion(m_id, m_PosX, m_PosY));
	}

	virtual ~EvtData_CreatePlayerExplosion()
	{
	}

	uint32_t m_id;
	uint32_t m_PosX;
	uint32_t m_PosY;

};

///////////////////////////////////////////////////////////////////////////////
/// <summary>
/// Points obtained events
/// </summary>
struct EvtData_PointsObtained : public BaseEventData
{

	static const EventType sk_EventType;
	virtual const EventType & GetEventType(void) const
	{
		return sk_EventType;
	}

	explicit EvtData_PointsObtained(const uint32_t &points)
		: m_Points(points)
	{}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr(GCC_NEW EvtData_PointsObtained(m_Points));
	}

	virtual ~EvtData_PointsObtained()
	{
	}

	uint32_t m_Points;

};

///////////////////////////////////////////////////////////////////////////////
/// <summary>
/// Lives remaining events
/// </summary>
struct EvtData_LivesRemaining : public BaseEventData
{

	static const EventType sk_EventType;
	virtual const EventType & GetEventType(void) const
	{
		return sk_EventType;
	}

	explicit EvtData_LivesRemaining(const uint32_t &lives)
		: m_Lives(lives)
	{}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr(GCC_NEW EvtData_LivesRemaining(m_Lives));
	}

	virtual ~EvtData_LivesRemaining()
	{
	}

	uint32_t m_Lives;

};

///////////////////////////////////////////////////////////////////////////////
/// <summary>
/// Play Sound Event
/// </summary>
struct EvtData_PlaySound : public BaseEventData
{

	static const EventType sk_EventType;
	virtual const EventType & GetEventType(void) const
	{
		return sk_EventType;
	}

	explicit EvtData_PlaySound(const std::string &filename)
		: m_Filename(filename)
	{}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr(GCC_NEW EvtData_PlaySound(m_Filename));
	}

	virtual ~EvtData_PlaySound()
	{
	}

	const std::string m_Filename;

};

///////////////////////////////////////////////////////////////////////////////
/// <summary>
/// Destroy Actor event
/// </summary>
struct EvtData_DestroyActor : public BaseEventData
{

	static const EventType sk_EventType;
	virtual const EventType & GetEventType(void) const
	{
		return sk_EventType;
	}

	explicit EvtData_DestroyActor(const uint32_t &actorId)
		: m_ActorId(actorId)
	{}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr(GCC_NEW EvtData_DestroyActor(m_ActorId));
	}

	virtual ~EvtData_DestroyActor()
	{
	}

	uint32_t m_ActorId;

};

///////////////////////////////////////////////////////////////////////////////
/// <summary>
/// Animation Finished event
/// </summary>
struct EvtData_AnimationFinished : public BaseEventData
{

	static const EventType sk_EventType;
	virtual const EventType & GetEventType(void) const
	{
		return sk_EventType;
	}

	explicit EvtData_AnimationFinished(const uint32_t &actorId)
		: m_ActorId(actorId)
	{}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr(GCC_NEW EvtData_AnimationFinished(m_ActorId));
	}

	virtual ~EvtData_AnimationFinished()
	{
	}

	uint32_t m_ActorId;

};

///////////////////////////////////////////////////////////////////////////////
/// <summary>
/// Enemy Collision Detected
/// </summary>
struct EvtData_EnemyCollisionDetected : public BaseEventData
{

	static const EventType sk_EventType;
	virtual const EventType & GetEventType(void) const
	{
		return sk_EventType;
	}

	explicit EvtData_EnemyCollisionDetected(const uint32_t &actorIdPlayer, const uint32_t &actorIdEnemy)
		: m_ActorPlayerId(actorIdPlayer), m_ActorEnemyId(actorIdEnemy)
	{}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr(GCC_NEW EvtData_EnemyCollisionDetected(m_ActorPlayerId, m_ActorEnemyId));
	}

	virtual ~EvtData_EnemyCollisionDetected()
	{
	}

	uint32_t m_ActorPlayerId;
	uint32_t m_ActorEnemyId;

};

///////////////////////////////////////////////////////////////////////////////
/// <summary>
/// Player Collision Detected
/// </summary>
struct EvtData_PlayerCollisionDetected : public BaseEventData
{

	static const EventType sk_EventType;
	virtual const EventType & GetEventType(void) const
	{
		return sk_EventType;
	}

	explicit EvtData_PlayerCollisionDetected(const uint32_t &actorIdPlayer, const uint32_t &actorIdEnemy)
		: m_ActorPlayerId(actorIdPlayer), m_ActorEnemyId(actorIdEnemy)
	{}


	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr(GCC_NEW EvtData_PlayerCollisionDetected(m_ActorPlayerId, m_ActorEnemyId));
	}

	virtual ~EvtData_PlayerCollisionDetected()
	{
	}

	uint32_t m_ActorPlayerId;
	uint32_t m_ActorEnemyId;

};
// IEventListener Description
//
// Base type for all event handler objects, note tht this is a
// C++ interface in the classic sense in that it defines the call
// sequence but does not provide any functionality nor aggregate
// any data.
//
// Thus, this is suitable for both single-inheritance use and MAY
// be used safely in a mutiple-inheritance construct, neither is
// imposed or assumed.

class IEventListener
{
public:

	explicit IEventListener()
	{}

	virtual ~IEventListener()
	{}

	// Returns ascii-text name for this listener, used mostly for
	// debugging

	virtual char const * GetName(void) const = 0;

	// Return 'false' to indicate that this listener did NOT
	// consume the event, ( and it should continue to be
	// propogated )
	// 					
	// return 'true' to indicate that this listener consumed the
	//event, ( and it should NOT continue to be propgated )

	virtual bool HandleEvent(IEventData const & event) const = 0;
};

}