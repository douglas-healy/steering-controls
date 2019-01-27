#ifndef BUTTON_H
#define BUTTON_H

#include "types.h"

class Button
{
private:
    button_type     m_buttonType;
    clickHandler    m_singleClickHandler    = nullptr,
                    m_doubleClickHandler    = nullptr,
                    m_longClickHandler      = nullptr;

public:
    Button(button_type);
    void button_Init(button_type);
    void BUTTON_OnSingleClick(clickHandler);
    void BUTTON_OnDoubleClick(clickHandler);
    void BUTTON_OnLongClick(clickHandler);

    void BUTTON_HandleClick(click_type);
};

#endif
