// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#pragma warning(disable: 4786)


#include "widget_keyboard.h"

#include "widget_push_button.h"

#include "screen.h" // For text drawing.

namespace
{


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
*******************************************************************************/
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
		const tstring ksName(1, ks1[n]);
		WidgetPushButton& button =
			*new WidgetPushButton(*this, ksName);
		m_cpButton.push_back(&button);
		button.setText(ksName);
		button.setBounds(
			WRect(knInsetH + n * 16,
				knInsetV,
				15, 15));
		KeyEventAdapter& keyEventAdapter = *new KeyEventAdapter(ks1[n]);
		button.pressed.connect(&keyEventAdapter, &KeyEventAdapter::keyPressed);
	}}

	{for (int n = 0; n != ks2.size(); ++n)
	{
		const tstring ksName(1, ks2[n]);
		WidgetPushButton& button =
			*new WidgetPushButton(*this, ksName);
		m_cpButton.push_back(&button);
		button.setText(ksName);
		button.setBounds(
			WRect(knInsetH + 8 + n * 16,
				knInsetV + 16,
				15, 15));
		KeyEventAdapter& keyEventAdapter = *new KeyEventAdapter(ks2[n]);
		button.pressed.connect(&keyEventAdapter, &KeyEventAdapter::keyPressed);
	}}

	{for (int n = 0; n != ks3.size(); ++n)
	{
		const tstring ksName(1, ks3[n]);
		WidgetPushButton& button =
			*new WidgetPushButton(*this, ksName);
		m_cpButton.push_back(&button);
		button.setText(ksName);
		button.setBounds(
			WRect(knInsetH + 8 + 4 + n * 16,
				knInsetV + 16 + 16,
				15, 15));
		KeyEventAdapter& keyEventAdapter = *new KeyEventAdapter(ks3[n]);
		button.pressed.connect(&keyEventAdapter, &KeyEventAdapter::keyPressed);
	}}

	{for (int n = 0; n != ks4.size(); ++n)
	{
		const tstring ksName(1, ks4[n]);
		WidgetPushButton& button =
			*new WidgetPushButton(*this, ksName);
		m_cpButton.push_back(&button);
		button.setText(ksName);
		button.setBounds(
			WRect(knInsetH + 8 + 4 + 8 + n * 16,
				knInsetV + 16 + 16 + 16,
				15, 15));
		KeyEventAdapter& keyEventAdapter = *new KeyEventAdapter(ks4[n]);
		button.pressed.connect(&keyEventAdapter, &KeyEventAdapter::keyPressed);
	}}

	// Backspace
	WidgetPushButton& button =
		*new WidgetPushButton(*this, _T("backspace"));
	m_cpButton.push_back(&button);
	button.setText(_T("<-"));
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
