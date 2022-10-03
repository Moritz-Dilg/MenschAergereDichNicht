#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

#include <LED.hpp>

// TODO: Adjust brightness according to visibility throug plexiglass
#define LED_BRIGHTNESS 50  // Brightness of the LEDs: 1 - 255

LED led = LED(LED_BRIGHTNESS);
u_int8_t n = 0;

void setup() {
    Serial.begin(9600);
    led.begin();
    pinMode(32, INPUT);
}

void loop() {
    if (digitalRead(32) == HIGH) {
        n++;
        if (n > 4) n = 0;
        led.setBase(n, BLUE);
        led.setBase(n, YELLOW);
        led.setBase(n, GREEN);
        led.setBase(n, RED);
        delay(200);
    }
}
