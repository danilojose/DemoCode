#pragma once

#include <System\Assert.h>
#include <System\StdLibraries.h>
#include <System\FileSystemReader.h>
#include <GameView\ImageResource.h>
#include <System\HashedString.h>
#include <functional>
using namespace Graphics;
namespace UI
{

	class UIWidget;
/// <summary>
/// The factory that will help me create the widgets
/// </summary>
class UIWidgetFactory
{
protected:

#define REGISTER_WIDGET(X) m_builderTable[HashedString(#X).getHashValue()]= [](){	return new X();};
	std::unordered_map<uint32_t, std::function<UIWidget*()>> m_builderTable;

public:
	void RegisterComponents();
	void UnRegisterComponents();
	UIWidget* Create(JSONNode * nodeDescriptor);
	
};
/// <summary>
/// Basic stuff for Implementing UIWidget in a future should increase complexity
/// </summary>
class UIWidget
{

private:
	enum class ValidType :uint8_t{
		VALID = 0,
		INVALID = 1
	};
	ValidType m_Valid;

protected:

	uint16_t									m_PosX;
	uint16_t									m_PosY;
	uint32_t									m_EntityId;
	std::string									m_Name;
public:

	/// <summary>
	/// Initializes a new instance of the <see cref="UIWidget"/> class.
	/// </summary>
	UIWidget() : m_PosX(0), m_PosY(0), m_EntityId(0),m_Valid(ValidType::VALID){}
	/// <summary>
	/// Initializes a new instance of the <see cref="UIWidget"/> class.
	/// </summary>
	/// <param name="positionX">The position x.</param>
	/// <param name="positionY">The position y.</param>
	UIWidget(uint32_t entityId,uint16_t positionX, uint16_t positionY) 
			: m_PosX(positionX), m_PosY(positionY), m_EntityId(entityId),m_Valid(ValidType::VALID){}

	virtual ~UIWidget() {};
	/// <summary>
	/// Determines whether this instance is valid.
	/// </summary>
	/// <returns></returns>
	inline bool IsValid() { return m_Valid == ValidType::VALID; }

	/// <summary>
	/// Sets the identifier.
	/// </summary>
	/// <param name="entityId">The entity identifier.</param>
	inline void SetId(uint32_t entityId) {
		ASSERT_DESCRIPTION(entityId != 0, "EntityId cannot be zero");
		m_EntityId = entityId;
		m_Valid = ValidType::VALID;
	}

	/// <summary>
	/// OnRenders this instance.
	/// </summary>
	virtual void OnRender()=0;

	/// <summary>
	/// Builds the specified node descriptor.
	/// </summary>
	/// <param name="nodeDescriptor">The node descriptor.</param>
	virtual void Build(JSONNode * nodeDescriptor);

};

/// <summary>
/// ImageWidget struct that contains the data that is read to display the FrontEnd
/// </summary>
class ImageWidget : public UIWidget
{
	protected:
	std::shared_ptr<ImageResource> m_Image;

	public:
	/// <summary>
	/// Prevents a default instance of the <see cref="ImageWidget"/> class from being created.
	/// </summary>
	ImageWidget() :UIWidget(){}

	/// <summary>
	/// Initializes a new instance of the <see cref="ImageData"/> struct.
	/// </summary>
	/// <param name="fileImage">The file image.</param>
	/// <param name="positionX">The position x.</param>
	/// <param name="positionY">The position y.</param>
	ImageWidget(uint32_t entityId, const std::string &fileImage, uint32_t positionX, uint32_t positionY);

	/// <summary>
	/// OnRenders this instance.
	/// </summary>
	virtual void OnRender() override;

	/// <summary>
	/// Builds this instance.
	/// </summary>
	virtual void Build(JSONNode * nodeDescriptor) override;

	/// <summary>
	/// Finalizes an instance of the <see cref="ImageData"/> class.
	/// </summary>
	virtual ~ImageWidget()
	{
	}


};
/// <summary>
/// LabelWidget class that contains the data that is read to display the FrontEnd
/// </summary>

class LabelWidget : public UIWidget
{
protected:
	std::string m_FontStyle;
	std::string m_Text;
	uint16_t m_Width;

public:
	/// <summary>
	/// Initializes a new instance of the <see cref="LabelData"/> struct.
	/// </summary>
	LabelWidget() :UIWidget() {};

	/// <summary>
	/// Initializes a new instance of the <see cref="LabelData"/> struct.
	/// </summary>
	/// <param name="fontStyle">The font file.</param>
	/// <param name="positionX">The position x.</param>
	/// <param name="positionY">The position y.</param>
	/// <param name="text">The text.</param>
	LabelWidget(uint32_t entityId,const std::string &fontStyle, uint32_t positionX, uint32_t positionY, uint16_t width,const std::string &text);

	/// <summary>
	/// OnRenders this instance.
	/// </summary>
	virtual void OnRender() override;

	/// <summary>
	/// OnRenders this instance.
	/// </summary>
	virtual void Build(JSONNode * nodeDescriptor) override;

	/// <summary>
	/// Finalizes an instance of the <see cref="LabelData"/> class.
	/// </summary>
	virtual  ~LabelWidget(){}

};

/// <summary>
/// LabelWidget class that contains the data that is read to display the FrontEnd
/// </summary>

class BindingLabelWidget : public LabelWidget
{
protected:
	std::unique_ptr<HashedString> m_Binding;

public:
	/// <summary>
	/// Initializes a new instance of the <see cref="LabelData"/> struct.
	/// </summary>
	BindingLabelWidget() :LabelWidget() {}


	/// <summary>
	/// Initializes a new instance of the <see cref="LabelData"/> struct.
	/// </summary>
	/// <param name="fontFile">The font file.</param>
	/// <param name="positionX">The position x.</param>
	/// <param name="positionY">The position y.</param>
	/// <param name="fontSize">Size of the font.</param>
	/// <param name="binding">The binding.</param>
	/// <param name="text">The default value of text.</param>
	/// <param name="bindingText">The text.</param>
	BindingLabelWidget(uint32_t entityId, const std::string &fontStyle, uint32_t positionX, uint32_t positionY, uint16_t width,const std::string &binding, const std::string &text,
		const std::string &bindingText);

	/// <summary>
	/// OnRenders this instance.
	/// </summary>
	virtual void OnRender() override;

	/// <summary>
	/// OnRenders this instance.
	/// </summary>
	virtual void Build(JSONNode * nodeDescriptor) override;

	/// <summary>
	/// Finalizes an instance of the <see cref="LabelData"/> class.
	/// </summary>
	virtual  ~BindingLabelWidget(){}

};
}