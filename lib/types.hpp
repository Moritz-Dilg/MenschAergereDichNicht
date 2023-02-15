#define N_PIXELS_FIELD 56
#define N_PIXELS_GOAL 16
#define N_PIXELS_BASE 26  // 16

#define P_BLUE 0
#define P_YELLOW 1
#define P_GREEN 2
#define P_RED 3

// TODO: Adjust brightness according to visibility throug plexiglass
#define LED_BRIGHTNESS 1			   // Brightness of the LEDs: 1 - 255
#define FIGURE_SELECT_TOGGLE_TIME 500  // Time in ms to toggle the selected LED

// LED Pins
#define LED_PIN_FIELD 33
#define LED_PIN_GOAL 25
#define LED_PIN_BASE 32

// Button Pins
#define PIN_BTN_C 14
#define PIN_BTN_B 27
#define PIN_BTN_A 26

// Button IDs
#define BTN_NONE 0
#define BTN_A 1
#define BTN_B 2
#define BTN_C 3

// TFT
#define GFX_BL 22
#define GFX_CS 5
#define GFX_DC 19
#define GFX_RST 21
#define GFX_SCK 18
#define GFX_MOSI 23
#define GFX_MISO GFX_NOT_DEFINED
#define GFX_BGR false