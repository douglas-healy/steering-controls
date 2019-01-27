#include <Arduino.h>
#include <TaskScheduler.h>

#include "../inc/types.h"
#include "../inc/button.h"
#include "../inc/jvc.h"

//-----------------------------------------------------
/// Definitions
#define DEBUG
#define SERIAL_BAUD_RATE                115200

#define TIME_CMD_INTERVAL               200
#define NUM_VOL_INCREMENTS              5

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
static inline void folderPrevious(void);

//-----------------------------------------------------
/// Enums
typedef enum
{
    TASK_VOL_UP_INDEF,
    TASK_VOL_DOWN_INDEF,
    TASK_VOL_UP_INCR,
    TASK_VOL_DOWN_INCR,
    TASK_INVALID,
};

//-----------------------------------------------------
/// Structs and arrays
static Button *buttons[BTN_INVALID] = {
    [BTN_MUTE]              = new Button(BTN_MUTE),
    [BTN_VOL_UP]            = new Button(BTN_VOL_UP), [BTN_VOL_DOWN] = new Button(BTN_VOL_DOWN),
    [BTN_SRC]               = new Button(BTN_SRC),
    [BTN_NEXT_TRACK]        = new Button(BTN_NEXT_TRACK),
    [BTN_PREVIOUS_TRACK]    = new Button(BTN_PREVIOUS_TRACK), };

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
    buttons[BTN_PREVIOUS_TRACK]->BUTTON_OnLongClick(&folderPrevious);

    JVC_Init(COMMAND_OUTPUT);
    runner.init();

    uint8_t i;
    for (i = 0; i < TASK_INVALID; ++i) { // add all tasks to the scheduler
        runner.addTask(*tasks[i]);
    }
}

void loop(void)
{
    runner.execute();
    click_event clickEvent = main_DetectClickEvent();
    if (clickEvent.btn != BTN_INVALID && clickEvent.click != INVALID_CLICK) {
        buttons[clickEvent.btn]->BUTTON_HandleClick(clickEvent.click);
    }
}

//-----------------------------------------------------
/// Function implementations
click_event main_DetectClickEvent(void)
{
    click_event clickEvent;

    uint16_t    adcLeftVoltage = analogRead(LEFT_RESISTOR_LADDER) * (5.0 / 1023.0),
                adcRightVoltage = analogRead(RIGHT_RESISTOR_LADDER) * (5.0 / 1023.0);

    // TODO: detect button click

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
    JVC_SendCommand(STEREO_MUTE);
    return;
}

void voiceControl(void)
{
    JVC_SendCommand(STEREO_VOICE_CONTROL);
    return;
}

void volumeUp(void)
{
    JVC_SendCommand(STEREO_VOL_UP);
    return;
}

void volumeDown(void)
{
    JVC_SendCommand(STEREO_VOL_DOWN);
    return;
}

void source(void)
{
    JVC_SendCommand(STEREO_SOURCE);
    return;
}

void equalizer(void)
{
    JVC_SendCommand(STEREO_EQUALIZER);
    return;
}

void power(void)
{
    JVC_SendCommand(STEREO_POWER);
    return;
}

void nextTrack(void)
{
    JVC_SendCommand(STEREO_NEXT_TRACK);
    return;
}

void previousTrack(void)
{
    JVC_SendCommand(STEREO_PREV_TRACK);
    return;
}

void folderForward(void)
{
    JVC_SendCommand(STEREO_FOLDER_FORW);
    return;
}

void folderPrevious(void)
{
    JVC_SendCommand(STEREO_FOLDER_BACK);
    return;
}
