#include "../inc/jvc.h"

#include <Arduino.h>
#include "../inc/types.h"

//-----------------------------------------------
/// Definitions
#define JVC_ADDRESS                 0x47
#define PULSE_WIDTH                 527 /// in microseconds

//-----------------------------------------------
/// Global Variables
pin_t                               m_output;
stereo_cmd_t                        m_address;

//-----------------------------------------------
/// Function prototypes
void jvc_SendHexCommand(unsigned char);
void jvc_SendValue(unsigned char);
void jvc_SendOne(void);
void jvc_SendZero(void);
void jvc_Preamble(void);
void jvc_Postamble(void);

void JVC_Init(pin_t output)
{
    m_output = output;
    m_address = JVC_ADDRESS;
    return;
}

void JVC_SendCommand(unsigned char value)
{
    jvc_SendHexCommand(value);
    jvc_SendHexCommand(value);
    return;
}

// Send a command to the radio, including the header, start bit, address and stop bits
void jvc_SendHexCommand(unsigned char value)
{
    unsigned char i;
    jvc_Preamble();                         // Send signals to precede a command to the radio
    for (i = 0; i < 3; ++i) {           // Repeat address, command and stop bits three times so radio will pick them up properly
        jvc_SendValue(m_address);               // Send the address
        jvc_SendValue((unsigned char) value);  // Send the command
        jvc_Postamble();                      // Send signals to follow a command to the radio
    }
    return;
}

// Send a value (7 bits, LSB is sent first, value can be an address or command)
void jvc_SendValue(unsigned char value)
{
    unsigned char i, tmp = 1;
    for (i = 0; i < sizeof(value) * 8 - 1; ++i) {
        if (value & tmp) {  // Do a bitwise AND on the value and tmp
            jvc_SendOne();
        } else {
            jvc_SendZero();
        }
        tmp = tmp << 1; // Bitshift left by 1
    }
    return;
}


// Signals to transmit a '0' bit
void jvc_SendZero(void)
{
    digitalWrite(m_output, HIGH);      // Output HIGH for 1 pulse width
    delayMicroseconds(PULSE_WIDTH);
    digitalWrite(m_output, LOW);       // Output LOW for 1 pulse width
    delayMicroseconds(PULSE_WIDTH);
    return;
}

// Signals to transmit a '1' bit
void jvc_SendOne(void)
{
    digitalWrite(m_output, HIGH);      // Output HIGH for 1 pulse width
    delayMicroseconds(PULSE_WIDTH);
    digitalWrite(m_output, LOW);       // Output LOW for 3 pulse widths
    delayMicroseconds(PULSE_WIDTH * 3);
    return;
}

// Signals to precede a command to the radio
void jvc_Preamble(void)
{
    // HEADER: always LOW (1 pulse width), HIGH (16 pulse widths), LOW (8 pulse widths)
    digitalWrite(m_output, LOW);       // Make sure output is LOW for 1 pulse width, so the header starts with a rising edge
    delayMicroseconds(PULSE_WIDTH * 1);
    digitalWrite(m_output, HIGH);      // Start of header, output HIGH for 16 pulse widths
    delayMicroseconds(PULSE_WIDTH * 16);
    digitalWrite(m_output, LOW);       // Second part of header, output LOW 8 pulse widths
    delayMicroseconds(PULSE_WIDTH * 8);

    // START BIT: always 1
    jvc_SendOne();
    return;
}

// Signals to follow a command to the radio
void jvc_Postamble(void)
{
    // STOP BITS: always 1
    jvc_SendOne();
    jvc_SendOne();
    return;
}
