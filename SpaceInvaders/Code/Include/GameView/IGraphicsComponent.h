#pragma once
#include <System\StdLibraries.h>
#include <GameView\RenderSystem.h>


namespace Graphics
{

//forward declarations
/// <summary>
/// Parent Component for a Graphics component
/// </summary>
class IGraphicsComponent
{
protected:
	uint32_t m_ActorId;
	uint32_t m_PosX;
	uint32_t m_PosY;

public:
	/// <summary>
	/// Initializes a new instance of the <see cref="IGraphicsComponent"/> class.
	/// </summary>
	/// <param name="ownerId">The owner identifier.</param>
	/// <param name="posX">The position x.</param>
	/// <param name="posY">The position y.</param>
	explicit IGraphicsComponent(uint32_t ownerId, uint32_t posX, uint32_t posY) :m_ActorId(ownerId), m_PosX(posX), m_PosY(posY)
	{
	}
	/// <summary>
	/// Finalizes an instance of the <see cref="IGraphicsComponent"/> class.
	/// </summary>
	virtual ~IGraphicsComponent(){}



	/// <summary>
	/// Updates the position.
	/// </summary>
	/// <param name="x">The x.</param>
	/// <param name="y">The y.</param>
	virtual void UpdatePosition(uint32_t x, uint32_t y)
	{
		m_PosX = x;
		m_PosY = y;
	}
	/// <summary>
	/// Abstract funciton that is in charge of the rendering. To be implemented by the child classes
	/// </summary>
	virtual void Render() = 0;
	/// <summary>
	/// Vs the get owner.
	/// </summary>
	/// <returns></returns>
	const uint32_t VGetOwner() const { return m_ActorId; }
	/// <summary>
	/// Vs the set owner.
	/// </summary>
	/// <param name="owner">The owner.</param>
	virtual void VSetOwner(uint32_t owner) { m_ActorId = owner; };
};

}