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
	game = new Game(tft);
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
			while (!game->turn())
				;
			break;

		case BTN_B:
			tft->showSettings();
			break;

		case BTN_C:
			// TODO: set tft to black
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