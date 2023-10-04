/**
 * @file EPD_7in3f.cpp
 * @brief 7.3inch e-Paper (F) Driver
 * @version 1.0
 * @date 2023-10-03
 * 
 * @author Sidpatchy
 */

#include "EPD_7in3f.h"
#include <Arduino.h>
#include <stdint.h>
#include <SPI.h>
#include "config.h"

/**
 * @brief Perform a software reset on the e-Paper.
 */
static void resetEPaper() {
    digitalWrite(RST_Pin, HIGH);
    delay(20);
    digitalWrite(RST_Pin, LOW);
    delay(2);
    digitalWrite(RST_Pin, HIGH);
    delay(20);
}

/**
 * @brief Send a command to the e-Paper.
 * 
 * @param command Command to be sent.
 */
static void sendCommand(uint8_t command) {
    digitalWrite(DC_Pin, LOW);
    digitalWrite(CS_Pin, LOW);
    SPI.transfer(command);
    digitalWrite(CS_Pin, HIGH);
}

/**
 * @brief Send data to the e-Paper.
 * 
 * @param data Data to be sent.
 */
static void sendData(uint8_t data) {
    digitalWrite(DC_Pin, HIGH);
    digitalWrite(CS_Pin, LOW);
    SPI.transfer(data);
    digitalWrite(CS_Pin, HIGH);
}

/**
 * @brief Wait until the e-Paper is no longer busy (High).
 */
static void waitForNotBusyHigh() {
    int count = 0;
    while (!digitalRead(BUSY_Pin)) {
        delay(10);
        count++;
        if (count > 5000) {
            return;
        }
    }
}

/**
 * @brief Wait until the e-Paper is no longer busy (Low).
 */
static void waitForNotBusyLow() {
    while (digitalRead(BUSY_Pin)) {
        delay(5);
    }
}

/**
 * @brief Turn on the e-Paper display.
 */
static void turnOnDisplay() {
    sendCommand(0x04);  // POWER_ON
    waitForNotBusyHigh();

    sendCommand(0x12);  // DISPLAY_REFRESH
    sendData(0x00);
    waitForNotBusyHigh();

    sendCommand(0x02);  // POWER_OFF
    sendData(0X00);
    waitForNotBusyHigh();
}

/**
 * @brief Initialize the e-Paper register.
 */
void EPD_7IN3F::init() {
    resetEPaper();
    waitForNotBusyHigh();
    delay(30);

    // Hereafter, configuration commands can be sent to initialize the e-Paper.
    sendCommand(0xAA);    // CMDH
    sendData(0x49);
    sendData(0x55);
    sendData(0x20);
    sendData(0x08);
    sendData(0x09);
    sendData(0x18);

    sendCommand(0x01);
    sendData(0x3F);
    sendData(0x00);
    sendData(0x32);
    sendData(0x2A);
    sendData(0x0E);
    sendData(0x2A);

    sendCommand(0x00);
    sendData(0x5F);
    sendData(0x69);

    sendCommand(0x03);
    sendData(0x00);
    sendData(0x54);
    sendData(0x00);
    sendData(0x44); 

    sendCommand(0x05);
    sendData(0x40);
    sendData(0x1F);
    sendData(0x1F);
    sendData(0x2C);

    sendCommand(0x06);
    sendData(0x6F);
    sendData(0x1F);
    sendData(0x1F);
    sendData(0x22);

    sendCommand(0x08);
    sendData(0x6F);
    sendData(0x1F);
    sendData(0x1F);
    sendData(0x22);

    sendCommand(0x13);    // IPC
    sendData(0x00);
    sendData(0x04);

    sendCommand(0x30);
    sendData(0x3C);

    sendCommand(0x41);     // TSE
    sendData(0x00);

    sendCommand(0x50);
    sendData(0x3F);

    sendCommand(0x60);
    sendData(0x02);
    sendData(0x00);

    sendCommand(0x61);
    sendData(0x03);
    sendData(0x20);
    sendData(0x01); 
    sendData(0xE0);

    sendCommand(0x82);
    sendData(0x1E); 

    sendCommand(0x84);
    sendData(0x00);

    sendCommand(0x86);    // AGID
    sendData(0x00);

    sendCommand(0xE3);
    sendData(0x2F);

    sendCommand(0xE0);   // CCSET
    sendData(0x00); 

    sendCommand(0xE6);   // TSSET
    sendData(0x00);

    sendCommand(0x04);  // PWR on
    waitForNotBusyHigh();
}

/**
 * @brief Clear the e-Paper screen with the given colour.
 * 
 * @param colour Colour enum to fill the screen.
 */
void EPD_7IN3F::clear(Colour colour) {
    uint16_t width = (display_width % 2 == 0) ? (display_width / 2) : (display_width / 2 + 1);
    uint8_t colourValue = static_cast<uint8_t>(colour);

    sendCommand(0x10);
    for (uint16_t j = 0; j < display_height; j++) {
        for (uint16_t i = 0; i < width; i++) {
            sendData((colourValue << 4) | colourValue);
        }
    }
    turnOnDisplay();
}


/**
 * @brief Display a colour block pattern on the e-Paper.
 */
void EPD_7IN3F::show7Block() {
    const unsigned long segmentWidth = 100;
    const unsigned long segmentHeight = 240;

    Colour const colors[8] = {
        Colour::BLACK,
        Colour::BLUE,
        Colour::GREEN,
        Colour::ORANGE,
        Colour::RED,
        Colour::YELLOW,
        Colour::WHITE,
        Colour::WHITE
    };

    sendCommand(0x10);
    for (unsigned long segment = 0; segment < 2; ++segment) {
        for (unsigned long i = 0; i < segmentHeight; i++) {
            for (unsigned long k = segment * 4; k < (segment + 1) * 4; k++) {
                for (unsigned long j = 0; j < segmentWidth; j++) {
                    uint8_t colorValue = static_cast<uint8_t>(colors[k]);
                    sendData((colorValue << 4) | colorValue);
                }
            }
        }
    }

    turnOnDisplay();
}

/**
 * @brief Display an image on the e-Paper.
 * 
 * @param image Image data to be displayed.
 */
void EPD_7IN3F::display(const uint8_t *image) {
    uint16_t width = (display_width % 2 == 0) ? (display_width / 2) : (display_width / 2 + 1);

    sendCommand(0x10);
    for (uint16_t j = 0; j < display_height; j++) {
        for (uint16_t i = 0; i < width; i++) {
            sendData(pgm_read_byte(&image[i + j * width]));
        }
    }
    turnOnDisplay();
}

/**
 * @brief Enter deep sleep mode for the e-Paper.
 */
void EPD_7IN3F::enterDeepSleep() {
    sendCommand(0x07);  // DEEP_SLEEP
    sendData(0xA5);
}
