#ifndef BUTTON_H
#define BUTTON_H

#include "types.h"

//--------------------------------------------
/// Types
typedef void        (*clickHandler)(void);

typedef enum _click_type {
    CLICK_SINGLE,
    CLICK_DOUBLE,
    CLICK_LONG,

    CLICK_INVALID,
} click_type;

typedef enum _button_type {
    BTN_MUTE,
    BTN_VOL_UP,
    BTN_VOL_DOWN,
    BTN_SRC,
    BTN_NEXT_TRACK,
    BTN_PREVIOUS_TRACK,

    BTN_INVALID,
} button_type;

typedef struct _click_event {
    button_type     btn;
    click_type      click;
} click_event;


//--------------------------------------------
/// Class definition
class Button
{
private:
    clickHandler    m_singleClickHandler    = nullptr,
                    m_doubleClickHandler    = nullptr,
                    m_longClickHandler      = nullptr;

public:
    button_type     m_buttonType;
    input_pin_t     m_inputPin              = INPUT_INVALID;
    float           m_lowerLimit            = 0.0,
                    m_upperLimit            = 0.0;

    Button(button_type, input_pin_t, float, float);
    void button_Init(button_type, input_pin_t, float, float);
    void BUTTON_OnSingleClick(clickHandler);
    void BUTTON_OnDoubleClick(clickHandler);
    void BUTTON_OnLongClick(clickHandler);

    void BUTTON_HandleClick(click_type);
};

#endif
