#pragma once
#include <System\StdLibraries.h>
#include <Logic\IBehaviourComponent.h>
#include <Logic\GameLogic.h>
#include <System\Events.h>
#include <System\EntityEvents.h>
#include <System\EventManager.h>
#include <random>

///namespaces
namespace GameSystem
{
	class World;
}
// Typedef declarations and forward declarations.


namespace AI
{
	class MinerGameLogic;
	/// <summary>
	/// Listener to Entity Events
	/// </summary>
	class MinerLogicListener : public IEventListener
	{
		MinerGameLogic *m_MinerLogic;
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="EntitySystemListener"/> class.
		/// </summary>
		/// <param name="gameBoard">The game board.</param>
		explicit MinerLogicListener(MinerGameLogic * minerLogic) : IEventListener() { m_MinerLogic = minerLogic; }
		/// <summary>
		/// Gets the name.
		/// </summary>
		/// <returns></returns>
		virtual char const* GetName(void) const override { return "Miner Logic Listener"; }
		/// <summary>
		/// Handles the event. Entity Events right now... By now I am only creating fires, explosions and destroying actors (sic)
		/// </summary>
		/// <param name="event">The event.</param>
		/// <returns></returns>
		virtual bool HandleEvent(IEventData const & event) const override;

	};

	/// <summary>
	/// AI System that is in charge of the current logic of the Game and takes care of the behaviour components
	/// </summary>
	class MinerGameLogic : public GameLogic
	{

	protected:

		EventListenerPtr m_pMinerLogicListener;			// Entity event listener

	public:

		/// <summary>
		/// Initializes a new instance of the <see cref="GameLogic"/> class.
		/// </summary>
		MinerGameLogic(){
			m_RandomNumber.seed();
		}
		/// <summary>
		/// Finalizes an instance of the <see cref="GameLogic"/> class.
		/// </summary>
		virtual ~MinerGameLogic() {}
		/// <summary>
		/// Reset this instance.
		/// </summary>
		/// <param name="lives">The lives.</param>
		virtual void Reset(uint32_t lives) override;
		/// <summary>
		/// Requests the entity to move.
		/// </summary>
		/// <param name="entityId">The entity identifier.</param>
		/// <param name="targetPosition">The target position.</param>
		/// <param name="middlePosition">The middle position.</param>
		/// <returns></returns>
		virtual void RequestEntityToMove(uint32_t entityId, const std::pair<uint16_t, uint16_t> &targetPosition, const std::pair<uint16_t, uint16_t> &middlePosition);

		/// <summary>
		/// Releases the cell at a given position in the world
		/// </summary>
		/// <param name="position">The target position.</param>
		/// <returns></returns>
		virtual void ReleaseCell(const std::pair<uint16_t,uint16_t> & position);

		/// <summary>
		/// Called when Update is called from the mainloop
		/// </summary>
		/// <param name="elapsedTime">The elapsed time.</param>
		/// <returns></returns>
		virtual bool OnUpdate(uint32_t elapsedTime) override;

	};

}