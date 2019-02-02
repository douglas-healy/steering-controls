#include <Arduino.h>
#include <TaskScheduler.h>

#include "../inc/types.h"
#include "../inc/button.h"
#include "../inc/jvc.h"

//-----------------------------------------------------
/// Definitions
#define DEBUG
#define SERIAL_BAUD_RATE                115200

#define ADC_VREF                        (float) 5.0
#define ADC_RESOLUTION                  10 // adc resolution in bits

#define TIME_CMD_INTERVAL               200
#define NUM_VOL_INCREMENTS              5

#define BTN_DEBOUNCE_TIME               20
#define BTN_DC_GAP                      150
#define BTN_LONG_HOLD_TIME              600

//-----------------------------------------------------
/// Function prototypes
click_event main_DetectClickEvent(void);

static inline void task_volumeUpIndef(void);
static inline void task_volumeDownIndef(void);
static inline void task_volumeUpIncrement(void);
static inline void task_volumeDownIncrement(void);

static inline void mute(void);
static inline void voiceControl(void);
static inline void volumeUp(void);
static inline void volumeDown(void);
static inline void source(void);
static inline void equalizer(void);
static inline void power(void);
static inline void nextTrack(void);
static inline void previousTrack(void);
static inline void folderForward(void);
static inline void folderBackward(void);

//-----------------------------------------------------
/// Enums
typedef enum {
    TASK_VOL_UP_INDEF,
    TASK_VOL_DOWN_INDEF,
    TASK_VOL_UP_INCR,
    TASK_VOL_DOWN_INCR,

    TASK_INVALID,
};

//-----------------------------------------------------
/// Structs and arrays
static Button *buttons[BTN_INVALID] = {
    [BTN_MUTE]              = new Button(BTN_MUTE, input_pin[INPUT_LEFT_CTRLS], 2.735, 3.5),
    [BTN_VOL_UP]            = new Button(BTN_VOL_UP, input_pin[INPUT_LEFT_CTRLS], 1.99, 2.735),
    [BTN_VOL_DOWN]          = new Button(BTN_VOL_DOWN, input_pin[INPUT_LEFT_CTRLS], 0.0, 1.99),
    [BTN_SRC]               = new Button(BTN_SRC, input_pin[INPUT_RIGHT_CTRLS], 2.625, 3.5),
    [BTN_NEXT_TRACK]        = new Button(BTN_NEXT_TRACK, input_pin[INPUT_RIGHT_CTRLS], 1.81, 2.625),
    [BTN_PREVIOUS_TRACK]    = new Button(BTN_PREVIOUS_TRACK, input_pin[INPUT_RIGHT_CTRLS], 0.0, 1.81),
};

static Task *tasks[TASK_INVALID] = {
    [TASK_VOL_UP_INDEF]     = new Task(TIME_CMD_INTERVAL, TASK_FOREVER, &volumeUp),
    [TASK_VOL_DOWN_INDEF]   = new Task(TIME_CMD_INTERVAL, TASK_FOREVER, &volumeDown),
    [TASK_VOL_UP_INCR]      = new Task(TIME_CMD_INTERVAL, NUM_VOL_INCREMENTS, &volumeUp),
    [TASK_VOL_DOWN_INCR]    = new Task(TIME_CMD_INTERVAL, NUM_VOL_INCREMENTS, &volumeDown),
};

//-----------------------------------------------------
/// Global variables
Scheduler runner;

//-----------------------------------------------------
/// Arduino setup and loop
void setup(void)
{
    uint8_t i;
    for (i = 0; i < OUTPUT_INVALID; ++i) { // initialize output pins
        pinMode(output_pin[i], OUTPUT);
    }

    for (i = 0; i < INPUT_INVALID; ++i) { // initialize input pins
        pinMode(input_pin[i], INPUT);
    }

#ifdef DEBUG
    Serial.begin(SERIAL_BAUD_RATE);
    delay(10);
    Serial.println("Serial connection initialized");
#endif

    /// Button click handlers
    buttons[BTN_MUTE]->BUTTON_OnSingleClick(&mute);
    buttons[BTN_MUTE]->BUTTON_OnLongClick(&voiceControl);
    buttons[BTN_VOL_UP]->BUTTON_OnSingleClick(&volumeUp);
    buttons[BTN_VOL_UP]->BUTTON_OnLongClick(&task_volumeUpIndef);
    buttons[BTN_VOL_DOWN]->BUTTON_OnSingleClick(&volumeDown);
    buttons[BTN_VOL_DOWN]->BUTTON_OnLongClick(&task_volumeDownIndef);
    buttons[BTN_SRC]->BUTTON_OnSingleClick(&source);
    buttons[BTN_SRC]->BUTTON_OnDoubleClick(&equalizer);
    buttons[BTN_SRC]->BUTTON_OnLongClick(&power);
    buttons[BTN_NEXT_TRACK]->BUTTON_OnSingleClick(&nextTrack);
    buttons[BTN_NEXT_TRACK]->BUTTON_OnLongClick(&folderForward);
    buttons[BTN_PREVIOUS_TRACK]->BUTTON_OnSingleClick(&previousTrack);
    buttons[BTN_PREVIOUS_TRACK]->BUTTON_OnLongClick(&folderBackward);

    JVC_Init(output_pin[OUTPUT_JVC_STEREO]);
    runner.init();
#ifdef DEBUG
    Serial.println("Scheduler initialized");
#endif

    for (i = 0; i < TASK_INVALID; ++i) { // add all tasks to the scheduler
        runner.addTask(*tasks[i]);
    }
#ifdef DEBUG
    Serial.println("Tasks added to execution chain");
#endif
}

void loop(void)
{
    runner.execute();
    click_event clickEvent = main_DetectClickEvent();
    if (clickEvent.btn != BTN_INVALID && clickEvent.click != CLICK_INVALID) {

#ifdef DEBUG
        switch (clickEvent.click) {
            case CLICK_SINGLE:
                Serial.print("Single click");
                break;

            case CLICK_DOUBLE:
                Serial.print("Double click");
                break;

            case CLICK_LONG:
                Serial.print("Long click");
                break;

            default:
                break;
        }

        Serial.print(" for ");

        switch (clickEvent.btn) {
            case BTN_MUTE:
                Serial.println("mute");
                break;

            case BTN_VOL_UP:
                Serial.println("volume up");
                break;

            case BTN_VOL_DOWN:
                Serial.println("volume down");
                break;

            case BTN_SRC:
                Serial.println("source");
                break;

            case BTN_NEXT_TRACK:
                Serial.println("next track");
                break;

            case BTN_PREVIOUS_TRACK:
                Serial.println("previous track");
                break;

            default:
                break;
        }
#endif

        buttons[clickEvent.btn]->BUTTON_HandleClick(clickEvent.click);
    }
}

//-----------------------------------------------------
/// Function implementations
click_event main_DetectClickEvent(void)
{
    click_event             clickEvent;

    uint16_t                adcLeftVoltage  = analogRead(input_pin[INPUT_LEFT_CTRLS]) * (ADC_VREF / ((float) pow(2.0, ADC_RESOLUTION) - 1.0)),
                            adcRightVoltage = analogRead(input_pin[INPUT_RIGHT_CTRLS]) * (ADC_VREF / ((float) pow(2.0, ADC_RESOLUTION) - 1.0));

    static button_type      currentBtn = BTN_INVALID,
                            previousBtn = BTN_INVALID,
                            clickedBtn = BTN_INVALID;

    static uint8_t          i = 0,
                            waitingForDoubleClick = FALSE,
                            wasDoubleClicked = FALSE,
                            wasLongClicked = FALSE;

    static unsigned long    clickTime = millis(),
                            releaseTime = millis();

    // TODO: detect button click
    currentBtn = BTN_INVALID;
    for (i = 0; i < BTN_INVALID; ++i) {
        float voltageReading = (buttons[i]->m_inputPin == input_pin[INPUT_LEFT_CTRLS] ? adcLeftVoltage : adcRightVoltage);
        if (voltageReading > buttons[i]->m_lowerLimit && voltageReading < buttons[i]->m_upperLimit) {
            currentBtn = buttons[i]->m_buttonType;
        }
    }

    if (currentBtn == BTN_INVALID) {
        tasks[TASK_VOL_UP_INDEF]->disable();
        tasks[TASK_VOL_DOWN_INDEF]->disable();
    }

    if (waitingForDoubleClick) {
        if (currentBtn == previousBtn && currentBtn == BTN_INVALID && (millis() - releaseTime) > BTN_DC_GAP) { // dc gap, expired. single click
            waitingForDoubleClick = FALSE;
            clickEvent.btn = clickedBtn;
            clickEvent.click = CLICK_SINGLE;
            clickedBtn = BTN_INVALID;
        } else if (currentBtn != BTN_INVALID && previousBtn == BTN_INVALID) { // double click
            waitingForDoubleClick = FALSE;
            clickEvent.btn = clickedBtn;
            clickEvent.click = CLICK_DOUBLE;
            clickedBtn = BTN_INVALID;
        }
    } else if (currentBtn != BTN_INVALID && previousBtn == BTN_INVALID && (millis() - releaseTime) > BTN_DEBOUNCE_TIME) { // click initiated
        clickTime = millis();
        clickedBtn = currentBtn;
        waitingForDoubleClick = TRUE;
    } else if (currentBtn == BTN_INVALID && previousBtn != BTN_INVALID) { // button released
        releaseTime = millis();

    } else if (currentBtn == previousBtn && currentBtn == clickedBtn && (millis() - clickTime) > BTN_LONG_HOLD_TIME) { // long click
        waitingForDoubleClick = FALSE;
        wasLongClicked = TRUE;
        clickEvent.btn = clickedBtn;
        clickEvent.click = CLICK_LONG;
        clickedBtn = BTN_INVALID;
    }

    previousBtn = currentBtn;
    return clickEvent;
}

void task_volumeUpIndef(void)
{
    tasks[TASK_VOL_UP_INDEF]->restart();
    return;
}

void task_volumeDownIndef(void)
{
    tasks[TASK_VOL_DOWN_INDEF]->restart();
    return;
}

void task_volumeUpIncrement(void)
{
    tasks[TASK_VOL_UP_INCR]->restart();
    return;
}

void task_volumeDownIncrement(void)
{
    tasks[TASK_VOL_DOWN_INCR]->restart();
    return;
}

void mute(void)
{
    JVC_SendCommand(JVC_CMD_MUTE);
    return;
}

void voiceControl(void)
{
    JVC_SendCommand(JVC_CMD_VOICE_CTRL);
    return;
}

void volumeUp(void)
{
    JVC_SendCommand(JVC_CMD_VOLUP);
    return;
}

void volumeDown(void)
{
    JVC_SendCommand(JVC_CMD_VOLDOWN);
    return;
}

void source(void)
{
    JVC_SendCommand(JVC_CMD_SOURCE);
    return;
}

void equalizer(void)
{
    JVC_SendCommand(JVC_CMD_EQUALIZER);
    return;
}

void power(void)
{
    JVC_SendCommand(JVC_CMD_POWER);
    return;
}

void nextTrack(void)
{
    JVC_SendCommand(JVC_CMD_NEXT_TRACK);
    return;
}

void previousTrack(void)
{
    JVC_SendCommand(JVC_CMD_PREV_TRACK);
    return;
}

void folderForward(void)
{
    JVC_SendCommand(JVC_CMD_FOLDER_FORW);
    return;
}

void folderBackward(void)
{
    JVC_SendCommand(JVC_CMD_FOLDER_BACK);
    return;
}
