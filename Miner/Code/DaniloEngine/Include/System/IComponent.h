#pragma once
#include <System\FileSystemReader.h>
#include <System\HashedString.h>
#include <System\StdLibraries.h>
#include <System\Events.h>


namespace GameSystem
{

	class Entity;
	/// <summary>
	/// ComponentID is a class that stores the hash id that will be used to identify the component inside the EntitySystem communication.
	/// </summary>
	class ComponentId
	{
		protected:
			HashedString						m_Identifier;
		public:
			explicit ComponentId(const HashedString &hashId) :m_Identifier(hashId){ }
			bool IsValid()
			{
				return !m_Identifier.getStr().empty();
			}

			unsigned long GetHashId()
			{
				return m_Identifier.getHashValue();
			}

		
	};
	/// <summary>
	/// IComponent base class. Every single component will inherit from it. An Entity will be composed of these components
	/// </summary>
	class IComponent
	{
	protected:

		ComponentId									m_Id;
		Entity*										m_Entity;

	public:

		/// <summary>
		/// Initializes a new instance of the <see cref="IComponent"/> class.
		/// </summary>
		/// <param name="hashId">The hash identifier.</param>
		IComponent(const std::string &hashId, Entity *entity) :m_Id(HashedString(hashId.c_str())), m_Entity(entity) {}

		/// <summary>
		/// Sets the identifier.
		/// </summary>
		/// <param name="id">The identifier.</param>
		virtual inline void SetID(const char *id) {

			m_Id = ComponentId(HashedString(id));
		}

		/// <summary>
		/// Sets the identifier.
		/// </summary>
		/// <param name="id">The identifier.</param>
		virtual inline unsigned long GetHashId() {
			return m_Id.GetHashId();
		}
		/// <summary>
		/// Sets the Entity that is the owner of this component.
		/// </summary>
		/// <param name="id">The entity identifier.</param>
		virtual void SetEntity(Entity* entity) 
		{
			m_Entity = entity;
		}

		/// <summary>
		/// Gets the Entity that is the owner of this component.
		/// </summary>
		virtual Entity* GetEntity() {

			return m_Entity;
		}
		/// <summary>
		/// Builds the component using the specified descriptor.
		/// </summary>
		/// <param name="descriptor">The descriptor.</param>
		virtual void Build(JSONNode *descriptor) {}

		/// <summary>
		/// Clones the component
		/// </summary>
		/// <param name="descriptor">The descriptor.</param>
		virtual std::shared_ptr<IComponent> Clone(Entity* entity) = 0;

		/// <summary>
		/// OnEntityEvent: This method is in charge of handling the events that are raised inside the entity domain to communicate one component with another.
		/// </summary>
		/// <param name="descriptor">The descriptor.</param>
		virtual void OnEntityEvent(IEventData const & event) = 0;

	private:

		/// <summary>
		/// Operator=s the specified other.
		/// </summary>
		/// <param name="other">The other.</param>
		/// <returns></returns>
		IComponent& operator= (const IComponent &other) = delete;
		/// <summary>
		/// Prevents a default instance of the <see cref="IComponent"/> class from being created.
		/// </summary>
		/// <param name="other">The other.</param>
		IComponent(const IComponent &other) = delete;


	};


}