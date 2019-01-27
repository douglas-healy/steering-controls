#include "../inc/button.h"

Button::Button(button_type buttonType)
{
	this->BUTTON_Init(buttonType);
}

void Button::BUTTON_Init(button_type buttonType)
{
	this->m_buttonType = buttonType;
	return;
}

void Button::BUTTON_OnSingleClick(clickHandler singleClickHandler)
{
	this->m_singleClickHandler = singleClickHandler;
	return;
}

void Button::BUTTON_OnDoubleClick(clickHandler doubleClickHandler)
{
	this->m_doubleClickHandler = doubleClickHandler;
	return;
}

void Button::BUTTON_OnLongClick(clickHandler longClickHandler)
{
	this->m_longClickHandler = longClickHandler;
	return;
}

void Button::BUTTON_HandleClick(click_type clickType)
{
	switch (clickType) {
		case SINGLE_CLICK:
			this->m_singleClickHandler();
			break;

		case DOUBLE_CLICK:
			this->m_doubleClickHandler();
			break;

		case LONG_CLICK:
			this->m_longClickHandler();
			break;
		default:
			break;
	}

	return;
}
