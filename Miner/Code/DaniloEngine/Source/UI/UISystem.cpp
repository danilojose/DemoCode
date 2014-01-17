#include <UI\UISystem.h>

using namespace GameSystem;
using namespace AI;
extern RenderSystem* g_pRenderSystem;
extern GameLogic * g_pGameLogic;
UISystem * g_pUISystem = nullptr;

/// <summary>
/// Initializes a new instance of the <see cref="UISystem"/> class.
/// </summary>
/// <param name="gameOptions">The game options.</param>
UISystem::UISystem(std::shared_ptr<FontSystem> fontSystem): m_pFontSystem(fontSystem),m_LastWidgetId(0)
{

	m_WidgetFactory = std::unique_ptr<UIWidgetFactory>(GCC_NEW UIWidgetFactory());
	m_pUISystemListener = EventListenerPtr(GCC_NEW UISystemListener(this));
	g_pUISystem = this;
}

/// <summary>
/// Inits the font System
/// </summary>
/// <param name="descriptor">The descriptor.</param>
void UISystem::Init(JSONNode* descriptor)
{
	m_WidgetFactory->RegisterComponents();

	JSONNode* fontSystemDescriptor = descriptor->GetChild("FontSystem");
	m_pFontSystem->Init(fontSystemDescriptor);

	IEventManager::Get()->VAddListener(m_pUISystemListener, EvtData_PointsObtained::sk_EventType);
	IEventManager::Get()->VAddListener(m_pUISystemListener, EvtData_LivesRemaining::sk_EventType);


}

/// <summary>
/// Loads a FrontEnd
/// </summary>
void UISystem::LoadFrontEnd(const std::string &filename)
{
	FileSystemReader fileReader(filename.c_str());
	JSONFileSystemParser levelParser(fileReader.GetContents());
	levelParser.Init();

	ASSERT_DESCRIPTION(levelParser.IsValid(), filename+":: Was not properly parsed");
	JSONNode * widgetsDescriptor = levelParser.GetNode("hud");
	const std::vector<JSONNode*> widgets = widgetsDescriptor->GetChild("Widgets")->GetArrayValues();
	for (const auto& iter : widgets)
	{
		UIWidget* widget=m_WidgetFactory->Create(iter);
		ASSERT_DESCRIPTION(widget, "Widget is null what implies the widget was not properly built");
		widget->SetId(GetNewWidgetId());
		m_Widgets.push_back(std::unique_ptr<UIWidget>(widget));
	}

}

/// <summary>
/// Toggles the pause.
/// </summary>
/// <param name="active">The active.</param>
void UISystem::TogglePause(bool active)
{
}



/// <summary>
/// On render function.
/// </summary>
void UISystem::OnRender()
{
	for (const auto& iter : m_Widgets)
	{
		iter->OnRender();
	}

}
void UISystem::RegisterGameLogicBindings(const HashedString &hashId, std::function<std::string(AI::GameLogic*)> function)
{
	ASSERT_DESCRIPTION(m_GameLogicBindings.find(hashId.getHashValue())==m_GameLogicBindings.end(),"Trying to insert an already existing binding");
	m_GameLogicBindings[hashId.getHashValue()] = function;

	std::string prueba = function(g_pGameLogic);

}

/// <summary>
/// </summary>
std::string UISystem::GetBindingValue(const HashedString &hashId)
{
	ASSERT_DESCRIPTION(m_GameLogicBindings.find(hashId.getHashValue()) != m_GameLogicBindings.end(), "Binding not found");
	
	return m_GameLogicBindings[hashId.getHashValue()](g_pGameLogic);
}


/// <summary>
/// Handles the event that could affect the UISystem
/// </summary>
/// <param name="event">The event.</param>
/// <returns></returns>
bool UISystemListener::HandleEvent(IEventData const & event) const
{
	EventType eventType = event.GetEventType();

	return true;
}
