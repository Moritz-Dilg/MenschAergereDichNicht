/* #include <Adafruit_NeoPixel.h> */

#include "_game.hpp"

Buttons *buttons;
TFT_Display *tft;
Game *game;

void setup(void) {
	tft = new TFT_Display(LED_BRIGHTNESS);
	srand(time(NULL));
	Serial.begin(9600);
	Serial.println("main");

	buttons = new Buttons();
}

void loop() {
	tft->setButton(BTN_A, "Spiel starten");
	tft->setButton(BTN_B,
				   "Settings");	 // TODO: Translate to german (max 8 chars)
	tft->setButton(BTN_C, "Beenden");

	u_int8_t button;
	while ((button = Buttons::getPressedButton()) == BTN_NONE)
		;
	switch (button) {
		case BTN_A:
			game = new Game(tft);

			while (!game->turn())
				;

			Serial.println(
				"--------------------------------\nGame "
				"ended\n--------------------------------");

			// TODO: Print some useful things to display
			delay(2);

			// TODO: turn off all leds
			delete game;
			break;

		case BTN_B:
			tft->showSettings();
			break;

		case BTN_C:
			delete buttons;
			delete tft;
			delete game;
			while (1)
				;
			break;

		default:
			break;
	}
}