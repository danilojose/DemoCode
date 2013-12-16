#include <System\EventManager.h>
#include <GameView\RenderSystem.h>
#include <System\Assert.h>
#include <System\Application.h>
#include <3rdParty\System.h>

extern GameCodeApp * g_pApp;
SDL_Renderer* RenderSystem::m_Renderer = nullptr;
RenderSystem * g_pRenderSystem;
using namespace Graphics;
/// <summary>
/// Initializes a new instance of the <see cref="RenderSystem" /> class.
/// </summary>
/// <param name="window">The window.</param>
RenderSystem::RenderSystem(SDL_Window* window)
{
	m_pGraphicsComponentListener = EventListenerPtr(GCC_NEW RenderEntityListener(this));
	IEventManager::Get()->VAddListener(m_pGraphicsComponentListener, EvtData_UpdatePosition::sk_EventType);
	IEventManager::Get()->VAddListener(m_pGraphicsComponentListener, EvtData_DestroyActor::sk_EventType);
	IEventManager::Get()->VAddListener(m_pGraphicsComponentListener, EvtData_AnimationFinished::sk_EventType);
	m_RenderEntityList.clear();
	g_pRenderSystem = this;

	ASSERT_DESCRIPTION((window == nullptr), "SDL_Window cannot be null: " << SDL_GetError());
	m_Renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	ASSERT_DESCRIPTION((ren == nullptr), "SDL_Init renderer failed: " << SDL_GetError());

}
RenderSystem::~RenderSystem()
{
	SDL_DestroyRenderer(m_Renderer);
	m_Renderer = nullptr;
}
// Pausing
/// <summary>
/// Toggles the pause.
/// </summary>
/// <param name="active">The active.</param>
void RenderSystem::TogglePause(bool active)
{
	//TODO: Pending to implement
	g_pRenderSystem = nullptr;
}

/// <summary>
/// Resets this instance.
/// </summary>
void RenderSystem::Reset()
{
	m_RenderEntityList.clear();
}

/// <summary>
/// Adds the render entity.
/// </summary>
/// <param name="renderEntity">The render entity.</param>
void RenderSystem::AddRenderEntity(std::shared_ptr<IGraphicsComponent> renderEntity)
{
	ASSERT_DESCRIPTION(renderEntity->VGetOwner() > 0, "Attempted to add an actor with no valid ID");

	m_RenderEntityList[renderEntity->VGetOwner()] = renderEntity;

}
/// <summary>
/// Gets the render entity.
/// </summary>
/// <param name="id">The identifier.</param>
/// <returns></returns>
std::shared_ptr<IGraphicsComponent> RenderSystem::GetRenderEntity(const uint32_t id)
{
	auto it = m_RenderEntityList.find(id);
	if (it == m_RenderEntityList.end())
	{
		std::shared_ptr<IGraphicsComponent> null;
		return nullptr;
	}
	return it->second;

}
/// <summary>
/// Removes the render entity.
/// </summary>
/// <param name="id">The identifier.</param>
void RenderSystem::RemoveRenderEntity(uint32_t id)
{
	auto it = m_RenderEntityList.find(id);
	if (it != m_RenderEntityList.end())
		m_RenderEntityList.erase(it->first);

}

/// <summary>
/// OnPreRender function
/// </summary>
void RenderSystem::OnPreRender()
{
	ASSERT_RESULT_DECL(const int result, SDL_RenderClear(m_Renderer));
	ASSERT_DESCRIPTION(!result, "Clear Rendering failed: " << SDL_GetError());
}
/// <summary>
/// Called when Render function happens... It iterates through the Graphics components and Renders them
/// </summary>
void RenderSystem::OnRender()
{
	for (const auto &renderEntity : m_RenderEntityList)
	{
		renderEntity.second->Render();
	}
}
/// <summary>
/// OnPostRender function
/// </summary>
void RenderSystem::OnPostRender()
{
	ASSERT_RESULT_DECL(const int result, SDL_RenderPresent(m_Renderer));
	ASSERT_DESCRIPTION(!result, "Rendering did not properly update the system");
}

/// <summary>
/// Renders an Image Resource
/// </summary>
void RenderSystem::RenderImage(std::shared_ptr<ImageResource> &ImageResource, const int &posX,const int &posY)
{
	ASSERT_DESCRIPTION(m_surface, "No image surface exists! Did you forget to call LoadBmp?");
	ASSERT_DESCRIPTION(alpha >= 0.0f && alpha <= 1.0f, "alpha must be a value between 0.0 and 1.0! alpha = " << alpha);

	std::shared_ptr<ImageResHandle> imageResHandle = std::static_pointer_cast<ImageResHandle>(g_pApp->GetCache()->GetHandle(ImageResource.get()));

	SDL_Rect destinationRect;
	destinationRect.x = static_cast<Sint16>(posX);
	destinationRect.y = static_cast<Sint16>(posY);
	destinationRect.w = static_cast<Sint16>(imageResHandle->GetWidth());
	destinationRect.h = static_cast<Sint16>(imageResHandle->GetHeight());

	ASSERT_RESULT_DECL(const int error, SDL_RenderCopy(m_Renderer, imageResHandle->GetTexture(), NULL, &destinationRect));
	ASSERT_DESCRIPTION(error == 0, "SDL_RenderCopy failed: " << SDL_GetError());

}

/// <summary>
/// Handles the event currently registered as potentially listened by Rendering System
/// </summary>
/// <param name="event">The event.</param>
/// <returns></returns>
bool RenderEntityListener::HandleEvent(IEventData const & event) const
{

	EventType eventType = event.GetEventType();

	if (eventType == EvtData_UpdatePosition::sk_EventType)
	{
		EvtData_UpdatePosition const & ed = static_cast< const EvtData_UpdatePosition & >(event);
		std::shared_ptr<IGraphicsComponent> entity = m_RenderSystem->GetRenderEntity(ed.m_ActorId);
		if (entity)
		{
			entity->UpdatePosition(ed.m_PosX, ed.m_PosY);
			return true;
		}
		else
		{
			return false;
		}

	}

	else if (eventType == EvtData_DestroyActor::sk_EventType)
	{
		EvtData_DestroyActor const & ed = static_cast< const EvtData_DestroyActor & >(event);
		m_RenderSystem->RemoveRenderEntity(ed.m_ActorId);
		return true;
	}

	// Popup the event only... 
	if (eventType == EvtData_AnimationFinished::sk_EventType)
	{
		EvtData_AnimationFinished const & ed = static_cast< const EvtData_AnimationFinished & >(event);
		IEventManager::Get()->VQueueEvent(IEventDataPtr(GCC_NEW EvtData_DestroyActor(ed.m_ActorId)));
	}
	return false;
}

