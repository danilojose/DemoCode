#pragma once
#include <GameView\ImageResource.h>
#include <System\Events.h>
#include <System\EventManager.h>
#include <System\StdLibraries.h>
///namespaces

using namespace GameSystem;

namespace Graphics
{
	class IGraphicsComponent;
// Typedef declarations and forward declarations.
typedef std::map<uint32_t, std::shared_ptr<IGraphicsComponent> > GraphicsComponentMap;

/// <summary>
/// Rendering System in charge of receiving the graphics components and managing the Graphics Resources
/// </summary>
class RenderSystem
{

protected:
	GraphicsComponentMap m_RenderEntityList;
	EventListenerPtr m_pGraphicsComponentListener;			// AI event listener

	bool m_RenderDiagnostics;						// Are we rendering diagnostics?

	static SDL_Renderer* m_Renderer;

public:

	/// <summary>
	/// Initializes a new instance of the <see cref="RenderSystem"/> class.
	/// </summary>
	/// <param name="window">The window.</param>
	explicit RenderSystem(SDL_Window *window);
	/// <summary>
	/// Finalizes an instance of the <see cref="RenderSystem"/> class.
	/// </summary>
	virtual ~RenderSystem();

	// Pausing
	/// <summary>
	/// Toggles the pause.
	/// </summary>
	/// <param name="active">The active.</param>
	void TogglePause(bool active);

	/// <summary>
	/// Resets this instance.
	/// </summary>
	void Reset();

	/// <summary>
	/// Adds the render entity.
	/// </summary>
	/// <param name="renderEntity">The render entity.</param>
	virtual void AddRenderEntity(std::shared_ptr<IGraphicsComponent> renderEntity);
	/// <summary>
	/// Gets the render entity.
	/// </summary>
	/// <param name="id">The identifier.</param>
	/// <returns></returns>
	virtual std::shared_ptr<IGraphicsComponent> GetRenderEntity(const uint32_t id);
	/// <summary>
	/// Removes the render entity.
	/// </summary>
	/// <param name="id">The identifier.</param>
	virtual void RemoveRenderEntity(uint32_t id);

	/// <summary>
	/// PreRender Operations to be performed
	/// </summary>
	virtual void OnPreRender();
	/// <summary>
	/// Called when Render function happens... It iterates through the Graphics components and Renders them
	/// </summary>
	virtual void OnRender();
	/// <summary>
	/// PostRender Operations to be performed
	/// </summary>
	virtual void OnPostRender();

	/// <summary>
	/// Gets Renderer
	/// </summary>
	static SDL_Renderer* GetRenderer() { return m_Renderer; }

	/// <summary>
	/// Renders an Image Resource
	/// </summary>
	/// <param name="ImageResource">The ImageResource to be rendered.</param>
	/// <param name="posX">The position X.</param>
	/// <param name="posY">The position Y</param>
	virtual void RenderImage(std::shared_ptr<ImageResource> &ImageResource, const int &posX, const int &posY);

private:
	/// <summary>
	/// Prevents a default instance of the <see cref="RenderSystem"/> class from being created.
	/// </summary>
	/// <param name="other">The other.</param>
	RenderSystem(const RenderSystem &other);
	/// <summary>
	/// Operator=s the specified other.
	/// </summary>
	/// <param name="other">The other.</param>
	/// <returns></returns>
	const RenderSystem operator=(const RenderSystem &other);
};


/// <summary>
/// Listener Class in charge of receiving the Rendering Events that Rendering System should receive
/// </summary>
class RenderEntityListener : public IEventListener
{
	RenderSystem *m_RenderSystem;
public:
	/// <summary>
	/// Initializes a new instance of the <see cref="RenderEntityListener"/> class.
	/// </summary>
	/// <param name="renderSystem">The render system.</param>
	explicit RenderEntityListener(RenderSystem * renderSystem) : IEventListener() { m_RenderSystem = renderSystem; }
	/// <summary>
	/// Gets the name.
	/// </summary>
	/// <returns></returns>
	virtual char const* GetName(void) const override { return "RenderEntityListener"; }

	/// <summary>
	/// Handles the event currently registered as potentially listened by Rendering System
	/// </summary>
	/// <param name="event">The event.</param>
	/// <returns></returns>
	virtual bool HandleEvent(IEventData const & event) const override;
};

}