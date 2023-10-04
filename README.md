# EPD_7in3f
I'm unclear on whether I'm clueless, but I was unable to get the driver code provided by Waveshare working with the ESP32, but did get it working with a Raspberry Pi. Speaking of which, I don't understand why anyone would use an RPi that idles at 2-3 watts with an E-paper display. Regardless, I analyzed the Python code for the RPi and used it to create an Arduino library.

I've specifically tested this with an [Inland ESP32 Core Board](https://community.microcenter.com/kb/articles/652-inland-esp32-core-board-black-and-eco-friendly), but it should work on most (read: all) ESP32 dev boards and other Arduino devices.

# Pinout
| e-Paper HAT   |     | ESP32 |
| ------------  | --- | ----- |
| VCC (Grey)    | <-> | 3.3v  |
| GND (Brown)   | <-> | GND   |
| DIN (Blue)    | <-> | IO13  |
| CLK (Yellow)  | <-> | IO14  |
| CS (Orange)   | <-> | IO15  |
| DC (Green)    | <-> | IO23  |
| RST (White)   | <-> | IO33  |
| BUSY (Purple) | <-> | IO27  |
