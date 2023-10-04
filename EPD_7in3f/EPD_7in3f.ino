/**
 * @file EPD_7in3f.ino
 * @brief Main file for testing the display.
 * @version 1.0
 * @date 2023-10-03
 * 
 * @author Sidpatchy
 */

#include "EPD_7in3f.h"
#include "ExampleImage.h"
#include <SPI.h>
#include "config.h"

void setup() {
  Serial.begin(9600);

  pinMode(RST_Pin, OUTPUT);
  pinMode(DC_Pin, OUTPUT);
  pinMode(CS_Pin, OUTPUT);
  pinMode(BUSY_Pin, INPUT);

  // Initialize the SPI bus with custom pins
  SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));
  SPI.begin(CLK_Pin, MISO_Pin, DIN_Pin, CS_Pin);

  // Initialize e-Paper
  Serial.println("Intializing display...");
  EPD_7IN3F::init();

  // Clear the screen with white
  Serial.println("Clear display...");
  EPD_7IN3F::clear(Colour::WHITE);
  delay(10000);

  // Show 7 colour block test
  Serial.println("Show 7 block test...");
  EPD_7IN3F::show7Block();
  delay(10000);

  // Show image
  Serial.println("Show test image...");
  EPD_7IN3F::display(ExampleImageImage7colour);

  // Enter sleep mode
  EPD_7IN3F::enterDeepSleep();
  esp_deep_sleep_start();
}

void loop() {
  // put your main code here, to run repeatedly:

}
