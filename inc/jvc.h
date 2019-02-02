#ifndef JVC_H
#define JVC_H

#include "types.h"

//--------------------------------------------
/// Types
typedef unsigned char                   jvc_cmd_t;

typedef enum _jvc_cmd {
    JVC_ADDRESS                         = (jvc_cmd_t) 0x47,
    JVC_CMD_POWER                       = (jvc_cmd_t) 0x0A,
    JVC_CMD_VOLUP                       = (jvc_cmd_t) 0x04,
    JVC_CMD_VOLDOWN                     = (jvc_cmd_t) 0x05,
    JVC_CMD_SOURCE                      = (jvc_cmd_t) 0x08,
    JVC_CMD_EQUALIZER                   = (jvc_cmd_t) 0x0D,
    JVC_CMD_MUTE                        = (jvc_cmd_t) 0x0E,
    JVC_CMD_NEXT_TRACK                  = (jvc_cmd_t) 0x12,
    JVC_CMD_PREV_TRACK                  = (jvc_cmd_t) 0x13,
    JVC_CMD_FOLDER_FORW                 = (jvc_cmd_t) 0x14,
    JVC_CMD_FOLDER_BACK                 = (jvc_cmd_t) 0x15,
    JVC_CMD_VOICE_CTRL                  = (jvc_cmd_t) 0x1A,
} jvc_cmd;

//--------------------------------------------
/// Function prototypes
void JVC_Init(pin_t);
void JVC_SendCommand(unsigned char);

#endif
