#pragma once

#include <Logic\IBehaviourComponent.h>
#include <System\World.h>
#include <System\Events.h>
#include <System\EventManager.h>
#include <System\Entity.h>
using namespace GameSystem;
namespace AI
{

	/// <summary>
	/// User controlled behaviour. This is the Behaviour that reacts to user input like Keyboard
	/// </summary>
	class StoneBehaviour : public IBehaviourComponent
	{
		enum class AISTATE : char { IDLE = 1, FALLING = 2, MOVING = 3 };
	private:
		AISTATE m_AIState;
		uint16_t m_TimeBonus;
		// TODO change to a NodeGraph composed by Node* (sic)
		std::vector<std::pair<uint16_t, uint16_t>> m_AIPath;
		std::vector<std::pair<uint16_t, uint16_t>>::iterator m_CurrentNode;
	public:

		static const std::string COMPONENT_NAME;
		/// <summary>
		/// Initializes a new instance of the <see cref="GameBoardBehaviour"/> class.
		/// </summary>
		StoneBehaviour() :IBehaviourComponent(COMPONENT_NAME), m_AIState(AISTATE::IDLE)
		{
		}
		/// <summary>
		/// Initializes a new instance of the <see cref="GameBoardBehaviour"/> class.
		/// </summary>
		/// <param name="owner">The entity owner.</param>
		StoneBehaviour(GameSystem::Entity *owner,uint32_t points,uint16_t timeBonus):IBehaviourComponent(COMPONENT_NAME,owner,points)
			, m_AIState(AISTATE::IDLE), m_TimeBonus(timeBonus)
		{
		}
		/// <summary>
		/// Finalizes an instance of the <see cref="GameBoardBehaviour"/> class.
		/// </summary>
		virtual ~StoneBehaviour(){
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
		virtual std::shared_ptr<IComponent> Clone(Entity *entity) override;

		/// <summary>
		/// OnEntityEvent: This method is in charge of handling the events that are raised inside the entity domain to communicate one component with another.
		/// </summary>
		/// <param name="descriptor">The descriptor.</param>
		virtual void OnEntityEvent(IEventData const & event) override;

		/// <summary>
		/// Evaluates the hit.
		/// </summary>
		/// <param name="targetPosition">The target position.</param>
		/// <param name="middlePosition">The middle position.</param>
		void CreateAIPath(const std::pair<uint16_t, uint16_t> &targetPosition, const std::pair<uint16_t, uint16_t> &middlePosition);


	private:
		void SwapCellEntities(const uint32_t &entityOne, const uint32_t &entityTwo);
	};



}