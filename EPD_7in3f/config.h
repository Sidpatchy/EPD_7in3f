/**
 * @file config.h
 * @brief Configuration section
 * @version 1.0
 * @date 2023-10-03
 * 
 * @author Sidpatchy
 */

#pragma once

// Specify custom SPI pins
constexpr int CLK_Pin = 14;    // Clock
constexpr int MISO_Pin = -1;   // MISO (not used here, hence -1)
constexpr int DIN_Pin = 13;    // MOSI
constexpr int RST_Pin = 33;    // Reset
constexpr int DC_Pin = 23;     // DC
constexpr int CS_Pin = 15;     // CS/SS
constexpr int BUSY_Pin = 27;   // BUSY
