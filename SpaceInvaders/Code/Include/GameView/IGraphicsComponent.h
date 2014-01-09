#pragma once
#include <System\StdLibraries.h>
#include <GameView\RenderSystem.h>
#include <System\IComponent.h>


namespace Graphics
{

//forward declarations
/// <summary>
/// Parent Component for a Graphics component
/// </summary>
class IGraphicsComponent :public IComponent
{
protected:
	RenderSystem *m_pRenderSystem;

public:

	/// <summary>
	/// Initializes a new instance of the <see cref="IGraphicsComponent"/> class.
	/// </summary>
	/// <param name="ownerId">The owner identifier.</param>
	/// <param name="posX">The position x.</param>
	/// <param name="posY">The position y.</param>
	IGraphicsComponent(const std::string & componentId) :IComponent(componentId,nullptr),m_pRenderSystem(nullptr)
	{
	}

	/// <summary>
	/// Initializes a new instance of the <see cref="IGraphicsComponent"/> class.
	/// </summary>
	/// <param name="ownerId">The owner identifier.</param>
	/// <param name="posX">The position x.</param>
	/// <param name="posY">The position y.</param>
	IGraphicsComponent(const std::string & componentId, Entity *owner, RenderSystem *renderSystem) :IComponent(componentId,owner),
								m_pRenderSystem(renderSystem)
	{
	}
	/// <summary>
	/// Finalizes an instance of the <see cref="IGraphicsComponent"/> class.
	/// </summary>
	virtual ~IGraphicsComponent(){}

	/// <summary>
	/// Finalizes an instance of the <see cref="IGraphicsComponent"/> class.
	/// </summary>
	virtual void SetRenderSystem(RenderSystem *renderSystem)
	{
		m_pRenderSystem = renderSystem;
	}



	/// <summary>
	/// Updates the position. In this kind of component when the position gets updated the current sprite to be rendered gets updated too
	/// </summary>
	virtual void OnRender() =0;
};

}