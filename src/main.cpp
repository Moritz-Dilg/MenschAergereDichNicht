#include "_game.hpp"

Buttons *buttons;
TFT_Display *tft;
LED_CONTROLLER *led;
Game *game;

void playGame();

void setup(void) {
	tft = new TFT_Display();
	led = new LED_CONTROLLER(LED_BRIGHTNESS);
	led->begin();
	led->clearAll();

	Serial.begin(9600);
	Serial.println("main");

	buttons = new Buttons();
}

void loop() {
	tft->setTextTop("Mensch aergere Dich nicht");
	tft->setButton(BTN_A, "Spiel starten");
	tft->setButton(BTN_B,
				   "Settings");	 // TODO: Translate to german (max 8 chars)
	tft->setButton(BTN_C, "Beenden");
	// TODO: Only roll dice 3 times if cannot move any figure (goal!!)
	u_int8_t button;
	u_int8_t selected;
	while ((button = Buttons::getPressedButton()) == BTN_NONE)
		;
	switch (button) {
		case BTN_A:
			srand(time(NULL));
			playGame();
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

			tft->clearScreen();
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

void playGame() {
	int8_t winner;
	game = new Game(tft, led, 2);

	while ((winner = game->turn()) == -1)
		;

	Serial.println(
		"--------------------------------\nGame "
		"ended\n--------------------------------");

	Serial.println("Winner: " + String(winner));
	tft->clearScreen();

	char *winner_str;
	switch (winner) {
		case P_BLUE:
			winner_str = "Blau";
			break;
		case P_YELLOW:
			winner_str = "Gelb";
			break;
		case P_GREEN:
			winner_str = "Gruen";
			break;
		case P_RED:
			winner_str = "Rot";
			break;
		default:
			break;
	}
	TextLine text[] = {
		{winner_str, 1},
		{"hat gewonnen", 1},
	};
	tft->setTextCenterLeft(text, 2);

	delete game;
}