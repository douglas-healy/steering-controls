#include <Arduino.h>

#include "../inc/jvc.h"
#include "../inc/types.h"

//-----------------------------------------------
/// Definitions
#define PULSE_WIDTH 			527
#define ADDRESS					0x47

//-----------------------------------------------
/// Global Variables
stereo_cmd_t 					m_address;

//-----------------------------------------------
/// Function prototypes
void SendValue(unsigned char);
void SendCommand(unsigned char);
void SendOne();
void SendZero();
void Preamble();
void Postamble();

void JVC_Init(stereo_cmd_t address)
{
	m_address = address;
	return;
}

// Send a value (7 bits, LSB is sent first, value can be an address or command)
void SendValue(unsigned char value) {
  unsigned char i, tmp = 1;
  for (i = 0; i < sizeof(value) * 8 - 1; i++) {
    if (value & tmp)  // Do a bitwise AND on the value and tmp
      SendOne();
    else
      SendZero();
    tmp = tmp << 1; // Bitshift left by 1
  }
}

void JVC_SendCommand(unsigned char value)
{
	SendCommand(value);
	SendCommand(value);
}

// Send a command to the radio, including the header, start bit, address and stop bits
void SendCommand(unsigned char value) {
  unsigned char i;
  Preamble();                         // Send signals to precede a command to the radio
  for (i = 0; i < 3; i++) {           // Repeat address, command and stop bits three times so radio will pick them up properly
    SendValue(ADDRESS);               // Send the address
    SendValue((unsigned char)value);  // Send the command
    Postamble();                      // Send signals to follow a command to the radio
  }
}

// Signals to transmit a '0' bit
void SendZero()
{
//	Serial.println("zero");
	digitalWrite(COMMAND_OUTPUT, HIGH);      // Output HIGH for 1 pulse width
	delayMicroseconds(PULSE_WIDTH);
	digitalWrite(COMMAND_OUTPUT, LOW);       // Output LOW for 1 pulse width
	delayMicroseconds(PULSE_WIDTH);
}

// Signals to transmit a '1' bit
void SendOne()
{
//	Serial.println("one");
	digitalWrite(COMMAND_OUTPUT, HIGH);      // Output HIGH for 1 pulse width
	delayMicroseconds(PULSE_WIDTH);
	digitalWrite(COMMAND_OUTPUT, LOW);       // Output LOW for 3 pulse widths
	delayMicroseconds(PULSE_WIDTH * 3);
}

// Signals to precede a command to the radio
void Preamble() {
  // HEADER: always LOW (1 pulse width), HIGH (16 pulse widths), LOW (8 pulse widths)
  digitalWrite(COMMAND_OUTPUT, LOW);       // Make sure output is LOW for 1 pulse width, so the header starts with a rising edge
  delayMicroseconds(PULSE_WIDTH * 1);
  digitalWrite(COMMAND_OUTPUT, HIGH);      // Start of header, output HIGH for 16 pulse widths
  delayMicroseconds(PULSE_WIDTH * 16);
  digitalWrite(COMMAND_OUTPUT, LOW);       // Second part of header, output LOW 8 pulse widths
  delayMicroseconds(PULSE_WIDTH * 8);

  // START BIT: always 1
  SendOne();
}

// Signals to follow a command to the radio
void Postamble() {
  // STOP BITS: always 1
  SendOne();
  SendOne();
}
