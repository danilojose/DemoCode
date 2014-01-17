#pragma once
#include <System\StdLibraries.h>
#include <strstream>
#include <Logic\GameLogic.h>
#include <Logic\IBehaviourComponent.h>
#include <GameView\IGraphicsComponent.h>
#include <Physics\CollisionComponent.h>

/// Using namespace
using namespace AI;
using namespace Graphics;
using namespace Physics;


namespace GameSystem
{

	class EntityID
	{
		protected:
			enum class EntityIdType : int8_t{ INVALID = -1 };
			void SetId(int8_t id) { m_Id = id; }
			bool IsValid() { return !(m_Id == static_cast<int8_t>(EntityIdType::INVALID)); }

		public:
			EntityID() :m_Id(static_cast<int8_t>(EntityIdType::INVALID)){};

		private:
			int8_t m_Id;
	};

	/// <summary>
	/// Entity class that stores the different subcomponents. (pending to make them inherit a single interface component)
	/// </summary>
	class Entity
	{
	protected:

		uint32_t									m_EntityId;
		uint32_t									m_PosX;
		uint32_t									m_PosY;
		HashedString								m_EntityName;
		std::vector<std::shared_ptr<IComponent>>	m_Components;

		/// <summary>
		/// Sets the identifier.
		/// </summary>
		/// <param name="id">The identifier.</param>
		virtual void SetID(uint32_t id) {

			m_EntityId = id;
		}

	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="Entity"/> class.
		/// </summary>
		/// <param name="id">The identifier.</param>
		/// <param name="posX">The position x.</param>
		/// <param name="posY">The position y.</param>
		/// <param name="behaviour">The behaviour.</param>
		/// <param name="graphicsComponent">The graphics component.</param>
		/// <param name="collisionComponent">The collision component.</param>

		/// TODO: Añadir componentes con un método AddComponent para no tener constructores gigantescos.
		/// TODO: Pasarle matrices.
		/// template <T> AddComponent() {  push_back(T);} ?
		Entity(uint32_t id, uint32_t posX, uint32_t posY, const HashedString &entityName) : m_EntityId(id), m_PosX(posX), m_PosY(posY), m_EntityName(entityName)
		{
		}

		/// <summary>
		/// Finalizes an instance of the <see cref="Entity"/> class.
		/// </summary>
		virtual ~Entity(){
		}
		/// <summary>
		/// Gets the position x.
		/// </summary>
		/// <returns></returns>
		const int GetPosX() const  { return m_PosX; }
		/// <summary>
		/// Gets the position y.
		/// </summary>
		/// <returns></returns>
		const int GetPosY() const  { return m_PosY; }
		/// <summary>
		/// Sets the position x.
		/// </summary>
		/// <param name="posx">The posx.</param>
		void SetPosX(uint32_t posx) { m_PosX = posx; }
		/// <summary>
		/// Sets the position y.
		/// </summary>
		/// <param name="posy">The posy.</param>
		void SetPosY(uint32_t posy) { m_PosY = posy; }
		/// <summary>
		/// Gets the identifier.
		/// </summary>
		/// <returns></returns>
		const uint32_t GetID() const { return m_EntityId; }
		/// <summary>
		/// Gets the identifier.
		/// </summary>
		/// <returns></returns>
		const HashedString &GetEntityName() const { return m_EntityName; }
		//TODO: Still to decide what to do with the Update of an Entity
		/// <summary>
		/// Called when [update].
		/// </summary>
		/// <param name="deltaMilliseconds">The delta milliseconds.</param>
		virtual void OnUpdate(uint32_t deltaMilliseconds){}
		/// <summary>
		/// Initializes the specified in.
		/// </summary>
		/// <param name="in">The in.</param>
		/// <returns></returns>
		virtual bool Init(std::istrstream &in);
		/// <summary>
		/// Serializes the specified out.
		/// </summary>
		/// <param name="out">The out.</param>
		virtual void Serialize(std::ostrstream &out) const;

		/// <summary>
		/// Adds the component.
		/// </summary>
		/// <param name="component">The component.</param>
		virtual void AddComponent(std::shared_ptr<IComponent> component);

		/// <summary>
		/// Adds the component.
		/// </summary>
		/// <param name="component">The component.</param>
		virtual std::vector<std::shared_ptr<IComponent>> & GetComponents() { return m_Components; }

		/// <summary>
		/// Trigger event inside the Entity. Later to be moved to a deque to be updated every single tick
		/// </summary>
		/// <param name="event">The event.</param>
		virtual void TriggerInternalEvent(IEventDataPtr const & event);

	};

}