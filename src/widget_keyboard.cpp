// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#pragma warning(disable: 4786)


#include "widget_keyboard.h"

#include "widget_tool_button.h"

#include "screen.h" // For text drawing.

namespace
{


/*******************************************************************************
******************************************************************************/
class WidgetKeyButton : public WidgetButton
{


public:

	WidgetKeyButton()
	{
	}

	WidgetKeyButton(
		Widget& parent) :
		WidgetButton(parent)
	{
	}

	WidgetKeyButton(
		Widget& parent,
		const tstring& ksName) :
		WidgetButton(parent, ksName)
	{
	}


private:

	virtual
	void
	drawButton();


};


/*******************************************************************************
******************************************************************************/
void
WidgetKeyButton::drawButton()
{
	COLORREF colour =
		isDown() ?
			RGB(128 + 64, 128 + 64, 0) :
			RGB(0, 0, 255);

	// TODO Fix this.
	RECT screenRect = reinterpret_cast<const RECT&>(getGlobalBounds());
	++screenRect.top;
	--screenRect.bottom;
	getBackBuffer().DrawLine(
		screenRect.left + 1,
		screenRect.top - 1,
		screenRect.right - 2,
		screenRect.top - 1,
		colour,
		0,
		NULL);
	getBackBuffer().FillRect(
		&screenRect,
		colour,
		0,
		NULL);
	getBackBuffer().DrawLine(
		screenRect.left + 1,
		screenRect.bottom,
		screenRect.right - 2,
		screenRect.bottom,
		colour,
		0,
		NULL);

	// Use name for now.
	getBackBuffer().DrawText(
		getGlobalX() + getBounds().getWidth() / 2,
		getGlobalY() + getBounds().getHeight() / 2 - 2,
		getName().c_str(),
		Screen::getSystemFont(),
		GDDRAWTEXT_CENTER,
		NULL,
		0,
		NULL);
}


// Use this to adapt key events.
struct KeyEventAdapter : public sigslot::has_slots<>
{
	KeyEventAdapter(
		const int knCode) :
		m_nCode(knCode)
	{
	}

	void
	keyPressed()
	{
		Widget::dispatchEventKeyPress(m_nCode);
	}

	void
	keyReleased()
	{
		Widget::dispatchEventKeyRelease(m_nCode);
	}

	int m_nCode;

};


} // namespace


/*******************************************************************************
******************************************************************************/
void
WidgetKeyboard::init()
{
	const tstring ks1(_T("1234567890-"));
	const tstring ks2(_T("qwertyuiop"));
	const tstring ks3(_T("asdfghjkl"));
	const tstring ks4(_T("zxcvbnm"));

	const int knInsetH = 0;
	const int knInsetV = 0;

	{for (int n = 0; n != ks1.size(); ++n)
	{
		WidgetKeyButton& button =
			*new WidgetKeyButton(*this, tstring(1, ks1[n]));
		m_cpButton.push_back(&button);
		button.setBounds(
			WRect(knInsetH + n * 16,
				knInsetV,
				15, 15));
		KeyEventAdapter& keyEventAdapter = *new KeyEventAdapter(ks1[n]);
		button.pressed.connect(&keyEventAdapter, &KeyEventAdapter::keyPressed);
	}}

	{for (int n = 0; n != ks2.size(); ++n)
	{
		WidgetKeyButton& button =
			*new WidgetKeyButton(*this, tstring(1, ks2[n]));
		m_cpButton.push_back(&button);
		button.setBounds(
			WRect(knInsetH + 8 + n * 16,
				knInsetV + 16,
				15, 15));
		KeyEventAdapter& keyEventAdapter = *new KeyEventAdapter(ks2[n]);
		button.pressed.connect(&keyEventAdapter, &KeyEventAdapter::keyPressed);
	}}

	{for (int n = 0; n != ks3.size(); ++n)
	{
		WidgetKeyButton& button =
			*new WidgetKeyButton(*this, tstring(1, ks3[n]));
		m_cpButton.push_back(&button);
		button.setBounds(
			WRect(knInsetH + 8 + 4 + n * 16,
				knInsetV + 16 + 16,
				15, 15));
		KeyEventAdapter& keyEventAdapter = *new KeyEventAdapter(ks3[n]);
		button.pressed.connect(&keyEventAdapter, &KeyEventAdapter::keyPressed);
	}}

	{for (int n = 0; n != ks4.size(); ++n)
	{
		WidgetKeyButton& button =
			*new WidgetKeyButton(*this, tstring(1, ks4[n]));
		m_cpButton.push_back(&button);
		button.setBounds(
			WRect(knInsetH + 8 + 4 + 8 + n * 16,
				knInsetV + 16 + 16 + 16,
				15, 15));
		KeyEventAdapter& keyEventAdapter = *new KeyEventAdapter(ks4[n]);
		button.pressed.connect(&keyEventAdapter, &KeyEventAdapter::keyPressed);
	}}

	// Backspace
	WidgetKeyButton& button =
		*new WidgetKeyButton(*this, _T("<-"));
	m_cpButton.push_back(&button);
	button.setBounds(
		WRect(knInsetH + 8 + 4 + 8 + 8 * 16,
			knInsetV + 16 + 16 + 16,
			27,15));
	KeyEventAdapter& keyEventAdapter = *new KeyEventAdapter(VK_BACK);
	button.pressed.connect(&keyEventAdapter, &KeyEventAdapter::keyPressed);

	// Use backspace to size entire keyboard.
	setSize(WDim(button.getX() + button.getWidth() + knInsetH,
		button.getY() + button.getHeight() + knInsetV));
}
