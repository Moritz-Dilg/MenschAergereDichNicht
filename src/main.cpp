/* #include <Adafruit_NeoPixel.h> */

#include "_game.hpp"
// TODO: Allow to select other figure if base is empty

Buttons *buttons;
TFT_Display *tft;
LED_CONTROLLER *led;
Game *game;

void setup(void) {
	tft = new TFT_Display();
	led = new LED_CONTROLLER(LED_BRIGHTNESS);
	led->begin();
	led->clearAll();

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
	u_int8_t selected;
	while ((button = Buttons::getPressedButton()) == BTN_NONE)
		;
	switch (button) {
		case BTN_A:
			game = new Game(tft, led, 3);

			while (!game->turn())
				;

			Serial.println(
				"--------------------------------\nGame "
				"ended\n--------------------------------");

			// TODO: Print some useful things to display
			delay(2);

			delete game;
			break;

		case BTN_B:
			selected = 0;
			tft->showSettings(10 / 2.5, 4, selected);
			while (true) {
				u_int8_t button = Buttons::getPressedButton();
				if (button == BTN_A) {
					if (selected > 0) {
						selected--;
						tft->showSettings(10 / 2.5, 4, selected);
					}
				} else if (button == BTN_B) {
					if (selected < 2) {
						selected++;
						tft->showSettings(10 / 2.5, 4, selected);
					}
				} else if (button == BTN_C) {
					if (selected == 0) {
						// TODO: change brightness
					} else if (selected == 1) {
						// TODO: change player count
					} else if (selected == 2) {
						break;
					}
				}
			}

			// TODO: clear settings

			break;

		case BTN_C:
			led->clearAll();

			esp_sleep_enable_ext1_wakeup(WAKEUP_BITMASK,
										 ESP_EXT1_WAKEUP_ANY_HIGH);
			esp_deep_sleep_start();

			break;

		default:
			break;
	}
}