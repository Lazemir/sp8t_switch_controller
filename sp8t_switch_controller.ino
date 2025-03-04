#include <Vrekrer_scpi_parser.h>

// Define the control pins for the switch
const int PIN_A = 2;  // Control bit 0
const int PIN_B = 3;  // Control bit 1
const int PIN_C = 4;  // Control bit 2

// SCPI parser instance
SCPI_Parser instrument;

int channelNumber;

void select_channel(byte channelNumber) {
  // Convert integer channel number to binary and write to pins
  digitalWrite(PIN_A, channelNumber & 0x01);       // Least significant bit
  digitalWrite(PIN_B, (channelNumber >> 1) & 0x01); // Middle bit
  digitalWrite(PIN_C, (channelNumber >> 2) & 0x01); // Most significant bit
}

// Function to handle ":CHANnel <num>" command
void SelectChannel(SCPI_C commands, SCPI_P parameters, Stream& interface){
  if (parameters.Size() != 1) {
      Serial.print("Waiting for 1 parameter, got ");
      Serial.println(parameters.Size());
      return;
  }
   
  channelNumber = String(parameters.First()).toInt();
  // Channel number must be in range 0-7
  if (channelNumber < 0 || channelNumber > 7) {
      Serial.println("Channel out of range. Must be 0-7.");
      return;
  }

  select_channel(channelNumber);

  Serial.print("Channel set to ");
  Serial.println(channelNumber);
}

// Function to handle "*IDN?" command
void Identify(SCPI_C commands, SCPI_P parameters, Stream& interface) {
  // Respond with a mock identification string
  Serial.println("Arduino SCPI Controller, v1.0");
}

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  while (!Serial) {
      ; // Wait for the serial port to connect
  }

  // Configure control pins as outputs
  pinMode(PIN_A, OUTPUT);
  pinMode(PIN_B, OUTPUT);
  pinMode(PIN_C, OUTPUT);

  // Set initial states to LOW
  select_channel(0);

  // Define SCPI commands
  instrument.RegisterCommand(":CHANnel", &SelectChannel);
  instrument.RegisterCommand("*IDN?", &Identify);
}

void loop() {
  instrument.ProcessInput(Serial, "\n");
}
