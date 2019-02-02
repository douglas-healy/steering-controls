#include "../inc/button.h"

Button::Button(button_type buttonType, input_pin_t inputPin, float lowerLimit, float upperLimit)
{
    this->button_Init(buttonType, inputPin, lowerLimit, upperLimit);
}

void Button::button_Init(button_type buttonType, input_pin_t inputPin, float lowerLimit, float upperLimit)
{
    this->m_buttonType = buttonType;
    this->m_inputPin = inputPin;
    this->m_lowerLimit = lowerLimit;
    this->m_upperLimit = upperLimit;
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
        case CLICK_SINGLE:
            this->m_singleClickHandler();
            break;

        case CLICK_DOUBLE:
            this->m_doubleClickHandler();
            break;

        case CLICK_LONG:
            this->m_longClickHandler();
            break;

        default:
            break;
    }

    return;
}
