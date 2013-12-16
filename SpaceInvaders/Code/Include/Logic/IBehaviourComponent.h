#pragma once
#include <System\StdLibraries.h>

namespace AI
{
/// <summary>
/// IBehaviourComponent base class for implementing AI Behaviours
/// </summary>
class IBehaviourComponent
{
protected:
	
	uint32_t m_ActorId;
	bool m_Alive;
	uint32_t m_PosX;
	uint32_t m_PosY;
	uint16_t m_Points;
public:
	/// <summary>
	/// Initializes a new instance of the <see cref="IBehaviourComponent"/> class.
	/// </summary>
	/// <param name="actorId">The actor identifier.</param>
	/// <param name="posX">The position x.</param>
	/// <param name="posY">The position y.</param>
	IBehaviourComponent(uint32_t actorId, uint32_t posX, uint32_t posY,uint16_t points) :m_ActorId(actorId), m_Alive(true), m_PosX(posX), m_PosY(posY),m_Points(points){}
	/// <summary>
	/// Finalizes an instance of the <see cref="IBehaviourComponent"/> class.
	/// </summary>
	virtual ~IBehaviourComponent(){
	};
	/// <summary>
	/// Behaves the specified delta milliseconds.
	/// </summary>
	/// <param name="deltaMilliseconds">The delta milliseconds.</param>
	virtual void behave(uint32_t deltaMilliseconds) = 0;

	/// <summary>
	/// Gets the Owner.
	/// </summary>
	/// <returns></returns>
	const uint32_t VGetOwner() const { return m_ActorId; }

	/// <summary>
	/// Gets if the certain component is alive or not.
	/// </summary>
	/// <returns></returns>
	const bool VGetAlive() const { return m_Alive; }

	/// <summary>
	/// Sets the Alive property
	/// </summary>
	/// <param name="alive">The alive.</param>
	virtual void VSetAlive(bool alive) { m_Alive = alive; }

	/// <summary>
	/// Gets the Points property
	/// </summary>
	/// <param name="alive">The alive.</param>
	virtual const uint16_t VGetPoints() const { return m_Points; }

	/// <summary>
	/// Vs the get position x.
	/// </summary>
	/// <returns></returns>
	virtual const uint32_t VGetPositionX() const
	{
		return m_PosX;
	}

	/// <summary>
	/// Vs the get position y.
	/// </summary>
	/// <returns></returns>
	virtual const uint32_t VGetPositionY() const
	{
		return m_PosY;
	}
};
}