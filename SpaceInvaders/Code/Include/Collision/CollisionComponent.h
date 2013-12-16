
#pragma once
namespace Collision
{
/// <summary>
///  Component Class storing the Collision properties from a given actor.
///  We will use square collision boxes taking into account that the position x and y is the left top corner
/// </summary>
struct ICollisionComponent
{
protected:
	uint32_t m_ActorId;
	uint32_t m_PosX;
	uint32_t m_PosY;
	uint32_t m_Size;

public:
	/// <summary>
	/// Initializes a new instance of the <see cref="ICollisionComponent"/> struct.
	/// </summary>
	/// <param name="ownerId">The owner identifier.</param>
	/// <param name="posX">The position x.</param>
	/// <param name="posY">The position y.</param>
	/// <param name="size">The size.</param>
	explicit ICollisionComponent(uint32_t ownerId, uint32_t posX, uint32_t posY, uint32_t size)
	{
		m_ActorId = ownerId;
		m_PosX = posX;
		m_PosY = posY;
		m_Size = size;
	}

	/// <summary>
	/// Finalizes an instance of the <see cref="ICollisionComponent"/> class.
	/// </summary>
	virtual ~ICollisionComponent(){}

	/// <summary>
	/// Updates the position.
	/// </summary>
	/// <param name="x">The x.</param>
	/// <param name="y">The y.</param>
	virtual void UpdatePosition(uint32_t x, uint32_t y)
	{
		m_PosX = x+m_Size;
		m_PosY = y+m_Size;
	}
	/// <summary>
	/// Gets the position X
	/// </summary>
	/// <returns></returns>
	const uint32_t GetPosX() const { return m_PosX; }
	/// <summary>
	/// Gets the position y.
	/// </summary>
	/// <returns></returns>
	const uint32_t GetPosY() const { return m_PosY; }
	/// <summary>
	/// Gets the size.
	/// </summary>
	/// <returns></returns>
	const uint32_t GetSize() const { return m_Size; }
	/// <summary>
	/// Checks the collision.
	/// </summary>
	virtual void CheckCollision() {}
	/// <summary>
	/// Gets the owner.
	/// </summary>
	/// <returns></returns>
	const uint32_t GetOwner() const { return m_ActorId; }
	/// <summary>
	/// Sets the owner.
	/// </summary>
	/// <param name="owner">The owner.</param>
	virtual void SetOwner(uint32_t owner) { m_ActorId = owner; };
};
}