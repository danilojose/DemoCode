#pragma once

#include <Logic\IBehaviourComponent.h>
#include <System\World.h>
#include <System\Events.h>
#include <System\EventManager.h>

namespace AI
{
	/// <summary>
	/// User controlled behaviour. This is the Behaviour that reacts to user input like Keyboard
	/// </summary>
	class GameBoardBehaviour : public IBehaviourComponent
	{
		friend class GameBoardBehaviourListener;
	private:

		EventListenerPtr m_pEntityListener;			// Entity event listener
		std::vector<HashedString>  m_StonesVector;

	public:

		static const std::string COMPONENT_NAME;
		/// <summary>
		/// Initializes a new instance of the <see cref="GameBoardBehaviour"/> class.
		/// </summary>
		GameBoardBehaviour();
		/// <summary>
		/// Initializes a new instance of the <see cref="GameBoardBehaviour"/> class.
		/// </summary>
		/// <param name="owner">The entity owner.</param>
		/// <param name="selectedCellSound">The sound.</param>
		GameBoardBehaviour(Entity *owner);
		/// <summary>
		/// Finalizes an instance of the <see cref="GameBoardBehaviour"/> class.
		/// </summary>
		virtual ~GameBoardBehaviour(){
		}

		/// <summary>
		/// Behaves when the specified delta milliseconds have passed. It checks the output from the Input and depending on the fire rate decides if it can fire or not.
		/// </summary>
		/// <param name="deltaMilliseconds">The delta milliseconds.</param>
		virtual void OnUpdate(uint32_t deltaMilliseconds) override;

		/// <summary>
		/// Builds the specified Component using the specified descriptor.
		/// </summary>
		/// <param name="descriptor">The descriptor.</param>
		virtual void Build(JSONNode *descriptor) override;

		/// <summary>
		/// Clones the current Component
		/// </summary>
		/// <param name="descriptor">The descriptor.</param>
		virtual std::shared_ptr<IComponent> Clone() override;

		/// <summary>
		/// OnEntityEvent: This method is in charge of handling the events that are raised inside the entity domain to communicate one component with another.
		/// </summary>
		/// <param name="descriptor">The descriptor.</param>
		virtual void OnEntityEvent(IEventData const & event) override;

		/// <summary>
		/// Evaluates the hit.
		/// </summary>
		/// <param name="positionX">The position x.</param>
		/// <param name="positionY">The position y.</param>
		/// <param name="entityName">Name of the entity.</param>
		bool EvaluateHit(uint16_t positionX, uint16_t positionY, const HashedString & entityName);
		

	private:
		void SwapCellEntities(const uint32_t &entityOne, const uint32_t &entityTwo);
	};

	/// <summary>
	/// Listener to Entity Events
	/// </summary>
	class GameBoardBehaviourListener : public IEventListener
	{
		GameBoardBehaviour *m_GameBoard;
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="EntitySystemListener"/> class.
		/// </summary>
		/// <param name="gameBoard">The game board.</param>
		explicit GameBoardBehaviourListener(GameBoardBehaviour * gameBoard) : IEventListener() { m_GameBoard = gameBoard; }
		/// <summary>
		/// Gets the name.
		/// </summary>
		/// <returns></returns>
		virtual char const* GetName(void) const override { return "Game Board Listener"; }
		/// <summary>
		/// Handles the event. Entity Events right now... By now I am only creating fires, explosions and destroying actors (sic)
		/// </summary>
		/// <param name="event">The event.</param>
		/// <returns></returns>
		virtual bool HandleEvent(IEventData const & event) const override;
	};

}