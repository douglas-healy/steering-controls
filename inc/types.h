#ifndef TYPES_H
#define TYPES_H

#include <Arduino.h>

#ifndef TRUE
#define TRUE                            (1 == 1)
#define FALSE                           (1 == 0)
#endif

typedef uint8_t                         pin_t;

typedef enum _output_pin_t {
    OUTPUT_ONBOARD_LED,
    OUTPUT_JVC_STEREO,

    OUTPUT_INVALID,
} output_pin_t;

static output_pin_t output_pin[OUTPUT_INVALID] = {
    [OUTPUT_ONBOARD_LED]               = 13,
    [OUTPUT_JVC_STEREO]                = 8,
};

typedef enum _input_pin_t {
    INPUT_LEFT_CTRLS,
    INPUT_RIGHT_CTRLS,

    INPUT_INVALID,
} input_pin_t;

static input_pin_t input_pin[INPUT_INVALID] = {
    [INPUT_LEFT_CTRLS]                  = A0,
    [INPUT_RIGHT_CTRLS]                 = A1,
};

#endif
