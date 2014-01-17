#include <Physics\PhysicsSystem.h>

namespace Physics
{
/// <summary>
/// Class that is mainly our engine as it is the one that contains all the systems that are in charge of implementing our game
/// </summary>
class MinerPhysicsSystem : public PhysicsSystem
{
public:
	/// <summary>
	/// Initializes a new instance of the <see cref="MinerEngine"/> class.
	/// </summary>
	MinerPhysicsSystem() :PhysicsSystem(){}
	/// <summary>
	/// Finalizes an instance of the <see cref="MinerEngine"/> class.
	/// </summary>
	virtual ~MinerPhysicsSystem(){}

	/// <summary>
	/// This function for this game will be in charge of calculating if a stone should start falling or not
	/// </summary>
	/// <param name="deltaMilliseconds">The delta milliseconds.</param>
	/// <returns></returns>
	virtual bool OnUpdate(uint32_t deltaMilliseconds) override;
};
}