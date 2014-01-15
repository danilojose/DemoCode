#pragma once


#include <functional>
#include <System\StdLibraries.h>
#include <System\IComponent.h>


namespace GameSystem
{

/// <summary>
/// Component Manager in charge of registering and creating new components
/// </summary>
class ComponentManager
{
	friend class IComponent;
	template<class T>
	std::shared_ptr<IComponent> CreateComponent()
	{
		return std::shared_ptr<T> (GCC_NEW T());
	}
#define REGISTER_COMPONENT(X) m_builderTable[#X]= [](){	return std::shared_ptr<X>(new X());};


protected:

	std::unordered_map<std::string, std::function<std::shared_ptr<IComponent>()>> m_builderTable;


public:
	/// <summary>
	/// Initializes this instance.
	/// </summary>
	virtual void Init();
	/// <summary>
	/// Gets the new component.
	/// </summary>
	/// <param name="">The .</param>
	/// <param name="descriptor">The descriptor.</param>
	/// <returns></returns>
	std::shared_ptr<IComponent> GetNewComponent(const std::string &, JSONNode *descriptor);

};

}
