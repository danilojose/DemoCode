#include <UI\UIElements.h>
#include <UI\UISystem.h>
#include <GameView\RenderSystem.h>
#include <Logic\GameLogic.h>

using namespace UI;
using namespace AI;
extern RenderSystem * g_pRenderSystem;
extern UISystem * g_pUISystem;
extern GameLogic * g_pGameLogic;
void UIWidgetFactory::RegisterComponents()
{
	REGISTER_WIDGET(ImageWidget);
	REGISTER_WIDGET(LabelWidget);
	REGISTER_WIDGET(BindingLabelWidget);

}
void UIWidgetFactory::UnRegisterComponents()
{
	m_builderTable.clear();
}
UIWidget* UIWidgetFactory::Create(JSONNode * nodeDescriptor)
{
	const char * componentName = nodeDescriptor->GetString("WidgetType");
	const auto& iter=m_builderTable.find(HashedString(componentName).getHashValue());
	ASSERT_DESCRIPTION(iter != m_builderTable.end(), "Cmponent Not found in the list of components we provide support");
	UIWidget * widget = iter->second();
	widget->Build(nodeDescriptor);
	return widget;
}
void UIWidget::Build(JSONNode * nodeDescriptor)
{
	m_PosX=nodeDescriptor->GetUInteger("PosX");
	m_PosY=nodeDescriptor->GetUInteger("PosY");
}


ImageWidget::ImageWidget(uint32_t entityId, const std::string &fileImage, uint32_t positionX, uint32_t positionY) :UIWidget(entityId,positionX, positionY)
{
	m_Image = std::shared_ptr<ImageResource>(GCC_NEW ImageResource(fileImage));
}

/// <summary>
/// Renders this instance.
/// </summary>
void ImageWidget::OnRender()
{
	g_pRenderSystem->RenderImage(m_Image, m_PosX, m_PosY);
}


/// <summary>
/// Renders this instance.
/// </summary>
void ImageWidget::Build(JSONNode * nodeDescriptor)
{
	UIWidget::Build(nodeDescriptor);
//	const char *imageFile = nodeDescriptor->GetString("ImageFile");

	m_Image = std::shared_ptr<ImageResource>(GCC_NEW ImageResource("graphics\\BackGround.jpg"));

}


LabelWidget::LabelWidget(uint32_t entityId, const std::string &fontStyle, uint32_t positionX, uint32_t positionY, uint16_t width,const std::string &text)
						:UIWidget(entityId, positionX, positionY), m_FontStyle(fontStyle), m_Text(text), m_Width(width)
{
}

/// <summary>
/// OnRenders this instance.
/// </summary>
void LabelWidget::OnRender()
{
	g_pUISystem->GetFontSystem()->RenderText(m_FontStyle, m_Text, m_PosX, m_PosY, m_Width, 20, 1.0, 1.0, 0);
}

/// <summary>
/// OnRenders this instance.
/// </summary>
void LabelWidget::Build(JSONNode * nodeDescriptor)
{
	UIWidget::Build(nodeDescriptor);
	m_FontStyle = nodeDescriptor->GetString("FontStyle");
	m_Width = nodeDescriptor->GetUInteger("Width");
	m_Text = nodeDescriptor->GetString("Text");
}

BindingLabelWidget::BindingLabelWidget(uint32_t entityId, const std::string &fontStyle, uint32_t positionX, uint32_t positionY, uint16_t width,const std::string &binding, const std::string &text,
	const std::string &bindingText) :LabelWidget(entityId, fontStyle, positionX, positionY,width,text)
{

	m_Binding = std::unique_ptr<HashedString>(GCC_NEW HashedString(bindingText.c_str()));
}

/// <summary>
/// OnRenders this instance.
/// </summary>
void BindingLabelWidget::OnRender()
{
	std::string textValue = g_pUISystem->GetBindingValue(*m_Binding);
	g_pUISystem->GetFontSystem()->RenderText(m_FontStyle, textValue, m_PosX, m_PosY, m_Width, 20, 1.0, 1.0, 0);

}

/// <summary>
/// OnRenders this instance.
/// </summary>
void BindingLabelWidget::Build(JSONNode * nodeDescriptor)
{
	LabelWidget::Build(nodeDescriptor);
	m_Binding = std::unique_ptr<HashedString> (GCC_NEW HashedString(nodeDescriptor->GetString("Binding")));
}