#pragma once


#include <functional>
#include <System\StdLibraries.h>
#include <System\IComponent.h>


namespace GameSystem
{

	class Foo
	{
	public:
		void foo(const std::string &msg)
		{
		}
	};

	class Bar
	{
	public:
		void bar(const std::string &msg, const  std::string &suffix)
		{
		}
	};
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
#define REGISTER_COMPONENT(X) m_builderTable[#X]= [](){	return std::shared_ptr<IComponent>(new X());};


protected:

	std::map<std::string, std::function<std::shared_ptr<IComponent>()>> m_builderTable;


public:

	void Init();

	std::shared_ptr<IComponent> GetNewComponent(const std::string &, const IniValuesMap &descriptor);

};

}
