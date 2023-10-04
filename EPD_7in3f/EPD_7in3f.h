/**
 * @file EPD_7in3f.h
 * @brief 7.3inch e-Paper (F) Driver
 * @version 1.0
 * @date 2023-10-03
 * 
 * @author Sidpatchy
 */

#ifndef EPD_7IN3F_H
#define EPD_7IN3F_H

#include <cstdint>

// Display resolution
constexpr int display_width = 800;
constexpr int display_height = 480;

// Colour Index
enum class Colour : uint8_t {
    BLACK   = 0x0,  // 000
    WHITE   = 0x1,  // 001
    GREEN   = 0x2,  // 010
    BLUE    = 0x3,  // 011
    RED     = 0x4,  // 100
    YELLOW  = 0x5,  // 101
    ORANGE  = 0x6,  // 110
    CLEAN   = 0x7   // 111 - unavailable
};

class EPD_7IN3F {
public:
    static void init();
    static void clear(Colour colour);
    static void show7Block();
    static void display(const uint8_t *Image);
    static void enterDeepSleep();
};

#endif
