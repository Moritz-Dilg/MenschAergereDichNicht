#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

#include <LED.hpp>

// TODO: Adjust brightness according to visibility throug plexiglass
#define LED_BRIGHTNESS 1  // Brightness of the LEDs: 1 - 255

LED led = LED(LED_BRIGHTNESS);
u_int8_t n = 0;

void setup() {
    Serial.begin(9600);
    led.begin();
}

void loop() {
}
