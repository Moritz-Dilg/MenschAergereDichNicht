/* #include <Adafruit_NeoPixel.h> */

#include "_game.hpp"

TFT_Display *tft;
Game *game;

void setup(void) {
	tft = new TFT_Display(LED_BRIGHTNESS);
	srand(time(NULL));
	Serial.begin(9600);
	Serial.println("main");

	game = new Game(tft);
}

void loop() { game->turn(); }