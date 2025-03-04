#include <Vrekrer_scpi_parser.h>
#include <Adafruit_MCP23X17.h>
#include "SP8TSwitchController.h"

// Define pins for controlling the switch
const byte PIN_A = 2;  // Control bit 0
const byte PIN_B = 3;  // Control bit 1
const byte PIN_C = 4;  // Control bit 2

#define CHANNELS_NUM 16

// Create an instance of the SP8TSwitchController class
SP8TSwitchController switchController(PIN_A, PIN_B, PIN_C);

// SCPI parser instance
SCPI_Parser instrument;

Adafruit_MCP23X17 gpio_extender;

// Current channel number
int channelNumber = 0;

// Function to handle the command ":CHANnel <num>"
void SelectChannel(SCPI_C commands, SCPI_P parameters, Stream& interface) {
  if (parameters.Size() != 1) {
    Serial.print("Waiting for 1 parameter, got ");
    Serial.println(parameters.Size());
    return;
  }

  // Convert the first parameter to an integer and update the channel number
  channelNumber = String(parameters.First()).toInt();
  // Check if the channel number is within the valid range (0–7)
  if (channelNumber < 0 || channelNumber > 7) {
    Serial.println("Channel out of range. Must be 0-7.");
    return;
  }

  // Select the channel using SP8TSwitchController
  switchController.setChannel(channelNumber);

  Serial.print("Channel set to ");
  Serial.println(channelNumber);
}

// Function to handle the command "*IDN?"
void Identify(SCPI_C commands, SCPI_P parameters, Stream& interface) {
  // Response with identification string
  Serial.println("Arduino SP8T Switch Controller, v1.0");
}

void setupLED() {
  for (int pin = 0; pin < CHANNELS_NUM; ++pin) {
    gpio_extender.pinMode(pin, OUTPUT);
  }
}

void switchLED(const byte& pin, const byte& mode) {
  gpio_extender.pinMode(pin, mode);
}

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  while (!Serial) {
    ; // Wait for the serial port to connect
  }

  if (!gpio_extender.begin_I2C()) {
    Serial.println("Error.");
    while (1);
  }

  setupLED();

  // Register SCPI commands
  instrument.RegisterCommand(":CHANnel", &SelectChannel);
  instrument.RegisterCommand("*IDN?", &Identify);

  Serial.println("SP8T Switch Controller initialized.");
}

void loop() {
  // Process incoming SCPI commands
  instrument.ProcessInput(Serial, "\n");
}