// Thomas Diggs, Peyton Allen, Yevgeniy Gavrikov
// UAB - Fall 2023
// EE437

// Include Necessary Libraries
#include <Adafruit_MCP3008.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include "SD.h"
#include "SPI.h"

// Define Constants
#define i2C_LCD_ADDR 0x3F  // I2C address of the LCD
#define NEW_CS_PIN 2       // Chip Select pin for MCP3008
#define TX 6               // Transmit pin for SofwareSerial
#define RX 7               // Receive pin for SoftwareSerial

// Create a SoftwareSerial object for Bluetooth communication
SoftwareSerial BTSerial(RX, TX);

// Create a LiquidCrystal_I2C object for the LCD
LiquidCrystal_I2C lcd(i2C_LCD_ADDR, 20, 4);

// Define Variables
int dly = 500;
int channel1 = 0;
int channel2 = 1;
int channel3 = 2;
int channel4 = 3;
int channel5 = 4;
int channel6 = 5;
int channel7 = 6;
int channel8 = 7;

// Create an Adafruit_MCP3008 object for the MCP3008 ADC
Adafruit_MCP3008 mcp;

// Define variables for SD Card
const int CSpin = 10;    // Chip Select pin for the SD Card
String dataString = "";  // Holds the data to be written to the SD card
int voltval1 = 0;        // Value read from your first sensor
int voltval2 = 0;        // Value read from your second sensor
File sensorData;         // File object for SD Card
int count = 0;           // Counter for data recording

// Setup function
void setup() {
  BTSerial.begin(9600);  // Start SoftwareSerial for Bluetooth communication
  Serial.begin(9600);    // Start serial communication with the computer
  Serial.println("Maker's Digest: Ready");
  Serial.print("Initializing SD card...");

  mcp.begin(NEW_CS_PIN);  // Initialize MCP3008 ADC with the specified Chip Select pin

  lcd.init();  // Initialize the LCD
  lcd.backlight();

  // SD Card - see if the card is present and can be initialized:
  if (!SD.begin(CSpin)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
}

// Loop function
void loop() {
  char msg[50];

  // Read sensor values from MCP3008 ADC
  int val1 = mcp.readADC(channel1);
  int val2 = mcp.readADC(channel2);
  int val3 = mcp.readADC(channel3);
  int val4 = mcp.readADC(channel4);
  int val5 = mcp.readADC(channel5);
  int val6 = mcp.readADC(channel6);
  int val7 = mcp.readADC(channel7);
  int val8 = mcp.readADC(channel8);

  // Convert ADC values to voltages
  int voltval1 = (val1 / 1024.0) * 5000.0;
  int voltval2 = (val2 / 1024.0) * 5000.0;
  int voltval3 = (val3 / 1024.0) * 5000.0;
  int voltval4 = (val4 / 1024.0) * 5000.0;
  int voltval5 = (val5 / 1024.0) * 5000.0;
  int voltval6 = (val6 / 1024.0) * 5000.0;
  int voltval7 = (val7 / 1024.0) * 5000.0;
  int voltval8 = (val8 / 1024.0) * 5000.0;

  // Clamp voltage values to 0 if below 100 mV
  if (voltval1 > 100) {
    voltval1 = voltval1;
  } else {
    voltval1 = 0;
  }
  if (voltval2 > 100) {
    voltval2 = voltval2;
  } else {
    voltval2 = 0;
  }
  if (voltval3 > 100) {
    voltval3 = voltval3;
  } else {
    voltval3 = 0;
  }
  if (voltval4 > 100) {
    voltval4 = voltval4;
  } else {
    voltval4 = 0;
  }
  if (voltval5 > 100) {
    voltval5 = voltval5;
  } else {
    voltval5 = 0;
  }
  if (voltval6 > 100) {
    voltval6 = voltval6;
  } else {
    voltval6 = 0;
  }
  if (voltval7 > 100) {
    voltval7 = voltval7;
  } else {
    voltval7 = 0;
  }
  if (voltval8 > 100) {
    voltval8 = voltval8;
  } else {
    voltval8 = 0;
  }

  // Bluetooth communication - Send voltage values in hexadecimal
  //    this is so the data sent in below 20 characters
  BTSerial.print(voltval1, HEX);
  BTSerial.print("|");
  BTSerial.print(voltval2, HEX);
  BTSerial.print("|");
  BTSerial.print(voltval3, HEX);
  BTSerial.print("|");
  BTSerial.print(voltval4, HEX);
  BTSerial.print("|");
  BTSerial.print(voltval5, HEX);
  BTSerial.print("|");
  BTSerial.print(voltval6, HEX);
  BTSerial.print("|");
  BTSerial.print(voltval7, HEX);
  BTSerial.print("|");
  BTSerial.print(voltval8, HEX);

  // LCD update
  lcd.clear();

  // LCD Display
  lcd.setCursor(0, 0);
  lcd.print("C1: ");
  lcd.setCursor(3, 0);
  lcd.print(voltval1);

  lcd.setCursor(0, 1);
  lcd.print("C2: ");
  lcd.setCursor(3, 1);
  lcd.print(voltval2);

  lcd.setCursor(0, 2);
  lcd.print("C3: ");
  lcd.setCursor(3, 2);
  lcd.print(voltval3);

  lcd.setCursor(0, 3);
  lcd.print("C4: ");
  lcd.setCursor(3, 3);
  lcd.print(voltval4);

  lcd.setCursor(10, 0);
  lcd.print("C5: ");
  lcd.setCursor(13, 0);
  lcd.print(voltval5);

  lcd.setCursor(10, 1);
  lcd.print("C6: ");
  lcd.setCursor(13, 1);
  lcd.print(voltval6);

  lcd.setCursor(10, 2);
  lcd.print("C7: ");
  lcd.setCursor(13, 2);
  lcd.print(voltval7);

  lcd.setCursor(10, 3);
  lcd.print("C8: ");
  lcd.setCursor(13, 3);
  lcd.print(voltval8);

  // Repeat the LCD display setup for other channels (C2 to C8)

  // Additional LCD Formatting
  for (int i = 0; i < 4; i++) {
    lcd.setCursor(7, i);
    lcd.print("mV");

    lcd.setCursor(9, i);
    lcd.print("|");

    lcd.setCursor(18, i);
    lcd.print("mV");
  }

  delay(dly);

  // SD Card - Save data every 10 loops
  count = count + 1;

  if (count >= 10) {
    dataString = String(voltval1) + "," + String(voltval2) + "," + String(voltval3) + "," + String(voltval4) + "," + String(voltval5) + "," + String(voltval6) + "," + String(voltval7) + "," + String(voltval8);  // convert to CSV
    saveData();                                                                                                                                                                                                    // save to SD card
    delay(100);
    count = 0;
  }
}

// Function to save data to the SD card
void saveData() {
  // Open the data.csv file for writing
  sensorData = SD.open("data.csv", FILE_WRITE);

  // Check if the file is successfully opened
  if (sensorData) {
    // Check if the file is empty (new file)
    if (sensorData.size() == 0) {
      // If the file is empty, write the header line
      sensorData.println("Channel 1 (mV), Channel 2 (mV), Channel 3 (mV), Channel 4 (mV), Channel 5 (mV), Channel 6 (mV), Channel 7 (mV), Channel 8 (mV)");
    }
    // for (int i = 0; i < 25; ++i) {
    //   array[i];
    // }

    // Write the dataString to the file
    sensorData.println(dataString);

    // Close the file
    sensorData.close();
  } else {
    // If the file couldn't be opened, print an error message
    Serial.println("Error opening data.csv for writing!");

    // Attempt to create the file
    sensorData = SD.open("data.csv", FILE_WRITE);

    // Check if the file is successfully created
    if (sensorData) {
      // If the file is open, write the header line
      sensorData.println("Channel 1 (mV), Channel 2 (mV), Channel 3 (mV), Channel 4 (mV), Channel 5 (mV), Channel 6 (mV), Channel 7 (mV), Channel 8 (mV)");

      // Write the dataString to the file
      sensorData.println(dataString);

      // Close the file
      sensorData.close();

      Serial.println("data.csv created!");
    } else {
      // If the file still couldn't be created, print an error message
      Serial.println("Error creating data.csv!");
    }
  }
}
