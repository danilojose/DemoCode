#pragma once
#include <System\StdLibraries.h>
#include <System\HashedString.h>

namespace GameSystem
{


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
		
	};
	/// <summary>
	/// IComponent base class. Every single component will inherit from it. An Entity will be composed of these components
	/// </summary>
	class IComponent
	{
	protected:

		ComponentId m_Id;

	public:

		/// <summary>
		/// Initializes a new instance of the <see cref="IComponent"/> class.
		/// </summary>
		/// <param name="hashId">The hash identifier.</param>
		IComponent(const std::string &hashId) :m_Id(HashedString(hashId.c_str())) {}

		/// <summary>
		/// Sets the identifier.
		/// </summary>
		/// <param name="id">The identifier.</param>
		virtual void SetID(const char *id) {

			m_Id = ComponentId(HashedString(id));
		}


	private:

		IComponent() = delete;

	};


}