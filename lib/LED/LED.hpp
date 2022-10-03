#define N_PIXELS_FIELD 56
#define N_PIXELS_GOAL 16
#define N_PIXELS_BASE 26  // 16
#define LED_PIN_FIELD 33
#define LED_PIN_GOAL 31
#define LED_PIN_BASE 26  // 30

#define BLUE 0
#define YELLOW 1
#define GREEN 2
#define RED 3

#include <Adafruit_NeoPixel.h>

class LED {
   public:
    LED(u_int8_t brightness);
    ~LED();

    void begin();
    void setBase(u_int8_t n, u_int8_t color);
    void setGoal(u_int8_t n, u_int8_t color);

   private:
    bool verifyNPins(u_int8_t n) {
        return n >= 0 && n <= 4;
    }
    void clearSection(u_int8_t index, Adafruit_NeoPixel &strip) {
        for (u_int8_t i = index * 4; i < index * 4 + 4; i++) {
            strip.setPixelColor(i, strip.Color(0, 0, 0));
        }
    }

   protected:
    u_int8_t brightness;
};
