#include <LED.hpp>

// Configure brightness of the LEDs
LED::LED(uint8_t brightness) {
    this->brightness = brightness;
}

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel fieldStrip = Adafruit_NeoPixel(N_PIXELS_FIELD, LED_PIN_FIELD, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel goalStrip = Adafruit_NeoPixel(N_PIXELS_GOAL, LED_PIN_GOAL, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel baseStrip = Adafruit_NeoPixel(N_PIXELS_BASE, LED_PIN_BASE, NEO_GRB + NEO_KHZ800);

// Initialize LED library.
void LED::begin() {
    fieldStrip.begin();
    fieldStrip.setBrightness(this->brightness);
    fieldStrip.show();  // Initialize all pixels to 'off'

    goalStrip.begin();
    goalStrip.setBrightness(this->brightness);
    goalStrip.show();  // Initialize all pixels to 'off'

    baseStrip.begin();
    baseStrip.setBrightness(this->brightness);
    baseStrip.show();  // Initialize all pixels to 'off'
}

/**
 * @brief Set the base LEDs
 *
 * @param n Number of figures in the base
 * @param color Color to set, use the defines BLUE, YELLOW, GREEN, RED
 */
void LED::setBase(u_int8_t n, u_int8_t color) {
    char ledOrder[16] = {0, 1, 3, 2,
                         7, 6, 4, 5,
                         9, 8, 10, 11,
                         13, 14, 12, 15};

    if (!verifyNFigures(n)) return;
    clearSection(color, baseStrip);

    for (u_int8_t i = color * 4; i < color * 4 + n; i++) {
        switch (color) {
            case BLUE:
                baseStrip.setPixelColor(ledOrder[i], baseStrip.Color(0, 0, 255));
                break;

            case YELLOW:
                baseStrip.setPixelColor(ledOrder[i], baseStrip.Color(255, 255, 0));
                break;

            case GREEN:
                baseStrip.setPixelColor(ledOrder[i], baseStrip.Color(0, 255, 0));
                break;

            case RED:
                baseStrip.setPixelColor(ledOrder[i], baseStrip.Color(255, 0, 0));
                break;

            default:
                break;
        }
    }
    baseStrip.show();
}

/**
 * @brief Set the goal LEDs
 *
 * @param n Number of figures in the goal
 * @param color Color to set, use the defines BLUE, YELLOW, GREEN, RED
 */
void LED::setGoal(u_int8_t n, u_int8_t color) {
    if (!verifyNFigures(n)) return;
    clearSection(color, goalStrip);

    for (u_int8_t i = color * 4; i < color * 4 + n; i++) {
        switch (color) {
            case BLUE:
                goalStrip.setPixelColor(3 - i, goalStrip.Color(0, 0, 255));
                break;

            case YELLOW:
                goalStrip.setPixelColor(i, goalStrip.Color(255, 255, 0));
                break;

            case GREEN:
                goalStrip.setPixelColor(i, goalStrip.Color(0, 255, 0));
                break;

            case RED:
                goalStrip.setPixelColor(i, goalStrip.Color(255, 0, 0));
                break;

            default:
                break;
        }
    }
    goalStrip.show();
}

/**
 * @brief Move figure from Index `i` (starting at 0) `n` figures forward
 *
 * @param i Index of the LED to move
 * @param n Number of figures to move forward
 * @param color Color to set, use the defines BLUE, YELLOW, GREEN, RED
 */
void moveFigure(u_int8_t i, u_int8_t n, u_int8_t color) {
    fieldStrip.setPixelColor(i, fieldStrip.Color(0, 0, 0));
    switch (color) {
        case BLUE:
            fieldStrip.setPixelColor(i + n, fieldStrip.Color(0, 0, 255));
            break;

        case YELLOW:
            fieldStrip.setPixelColor(i + n, fieldStrip.Color(255, 255, 0));
            break;

        case GREEN:
            fieldStrip.setPixelColor(i + n, fieldStrip.Color(0, 255, 0));
            break;

        case RED:
            fieldStrip.setPixelColor(i + n, fieldStrip.Color(255, 0, 0));
            break;

        default:
            break;
    }
    fieldStrip.show();
}

// Deallocate LED object
LED::~LED() {
    fieldStrip.~Adafruit_NeoPixel();
    goalStrip.~Adafruit_NeoPixel();
    baseStrip.~Adafruit_NeoPixel();
}
