#pragma once

#include <Physics\MinerPhysicsSystem.h>
#include <Logic\MinerGameLogic.h>
#include <System\StdLibraries.h>
#include <System\EntitySystem.h>
#include <System\Application.h>

/// NameSpaces
using namespace AI;

namespace GameSystem
{
	/// <summary>
	/// Class that is mainly our engine as it is the one that contains all the systems that are in charge of implementing our game
	/// </summary>
	class MinerComponentManager : public ComponentManager
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="MinerEngine"/> class.
		/// </summary>
		MinerComponentManager() :ComponentManager(){}
		/// <summary>
		/// Finalizes an instance of the <see cref="MinerEngine"/> class.
		/// </summary>
		virtual ~MinerComponentManager(){}

		/// <summary>
		/// Initializes this instance.
		/// </summary>
		virtual void Init() override;

	};
	/// <summary>
	/// Class that is mainly our engine as it is the one that contains all the systems that are in charge of implementing our game
	/// </summary>
	class MinerEntitySystem : public EntitySystem
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="MinerEngine"/> class.
		/// </summary>
		MinerEntitySystem() :EntitySystem(std::unique_ptr<ComponentManager>(GCC_NEW MinerComponentManager())){}
		/// <summary>
		/// Finalizes an instance of the <see cref="MinerEngine"/> class.
		/// </summary>
		virtual ~MinerEntitySystem(){}

		/// <summary>
		/// Removes the entity.
		/// </summary>
		/// <param name="id">The identifier.</param>
		virtual void RemoveEntity(uint32_t id) override;
	};

	/// <summary>
	/// Class that is mainly our engine as it is the one that contains all the systems that are in charge of implementing our game
	/// </summary>
	class MinerEngine: public GameCodeApp
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="MinerEngine"/> class.
		/// </summary>
		MinerEngine() :GameCodeApp(std::unique_ptr<EntitySystem> (GCC_NEW MinerEntitySystem()),
			std::shared_ptr<MinerGameLogic>(GCC_NEW MinerGameLogic()),
			std::unique_ptr<MinerPhysicsSystem>(GCC_NEW MinerPhysicsSystem())
			){}
		/// <summary>
		/// Finalizes an instance of the <see cref="MinerEngine"/> class.
		/// </summary>
		virtual ~MinerEngine() {}

		/// <summary>
		/// Initializes the instance.
		/// </summary>
		/// <param name="screenWidth">Width of the screen.</param>
		/// <param name="screenHeight">Height of the screen.</param>
		/// <returns></returns>
		virtual bool InitInstance(int screenWidth = 1024, int screenHeight = 768) override;

		/// <summary>
		/// Registers the base game events. This is the single place where the Events are registered. To be called when initialized
		/// </summary>
		virtual void RegisterBaseGameEvents() override;

	};

}