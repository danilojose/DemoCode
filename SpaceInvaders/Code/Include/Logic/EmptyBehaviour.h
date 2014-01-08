#pragma once

#include <Logic\IBehaviourComponent.h>
#include <3rdParty\Input.h>
#include <System\Entity.h>
namespace AI
{
	/// <summary>
	/// This class is an utily class that I use sometimes to make tests
	/// </summary>
	class EmptyBehaviour : public IBehaviourComponent
	{
	private:

	public:
		static const std::string COMPONENT_NAME;
		/// <summary>
		/// Initializes a new instance of the <see cref="EmptyBehaviour"/> class.
		/// </summary>
		/// <param name="actorId">The actor identifier.</param>
		/// <param name="posX">The position x.</param>
		/// <param name="posY">The position y.</param>
		EmptyBehaviour(uint32_t actorId, uint32_t posX, uint32_t posY) :IBehaviourComponent(EmptyBehaviour::COMPONENT_NAME,actorId, posX, posY, 0){}
		/// <summary>
		/// Finalizes an instance of the <see cref="EmptyBehaviour"/> class.
		/// </summary>
		virtual ~EmptyBehaviour(){
		}
		/// <summary>
		/// Behaves the specified delta milliseconds.
		/// </summary>
		/// <param name="deltaMilliseconds">The delta milliseconds.</param>
		virtual void behave(uint32_t deltaMilliseconds) override {};

	};
}
