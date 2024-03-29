#include "_game.hpp"

Buttons *buttons;
TFT_Display *tft;
LED_CONTROLLER *led;
Game *game;

u_int8_t player_count = 4;
u_int8_t brightness = 50;

void playGame(u_int8_t player_count);
int changeSetting(u_int8_t min, u_int8_t max, u_int8_t inc, u_int8_t defaultValue, std::function<void(u_int8_t)> updateTFT);

void setup(void) {
	tft = new TFT_Display();
	led = new LED_CONTROLLER(50);
	led->begin();
	led->clearAll();

	Serial.begin(9600);
	Serial.println("main");

	buttons = new Buttons();
}

void loop() {
	tft->setTextTop("Mensch aergere Dich nicht");
	tft->setButton(BTN_A, "Spiel starten");
	tft->setButton(BTN_B, "Settings");
	tft->setButton(BTN_C, "Beenden");

	u_int8_t button;
	u_int8_t selected;

	while ((button = Buttons::getPressedButton()) == BTN_NONE)
		;
	switch (button) {
		case BTN_A:
			srand(time(NULL));
			playGame(player_count);
			break;

		case BTN_B:
			selected = 0;
			tft->setButton(BTN_A, "v");
			tft->setButton(BTN_B, "^");
			tft->setButton(BTN_C, "OK");
			tft->showSettings(brightness / 2.5, player_count, selected);
			while (true) {
				u_int8_t button = Buttons::getPressedButton();
				if (button == BTN_A) {
					if (selected < 2) {
						selected++;
						tft->showSettings(brightness / 2.5, player_count,
										  selected);
					}
				} else if (button == BTN_B) {
					if (selected > 0) {
						selected--;
						tft->showSettings(brightness / 2.5, player_count,
										  selected);
					}
				} else if (button == BTN_C) {
					if (selected == 0) {
						led->initField();
						auto updateTFT = [&](u_int8_t value) { led->setBrightness(value * 2.5); return tft->showSettings(value, player_count, selected); };
						brightness = changeSetting(1, 100, 10, brightness / 2.5, updateTFT) * 2.5;
						led->setBrightness(brightness);
						led->clearAll();
					} else if (selected == 1) {
						auto updateTFT = [&](u_int8_t value) { return tft->showSettings(brightness / 2.5, value, selected); };
						player_count = changeSetting(2, 4, 1, player_count, updateTFT);
					} else if (selected == 2) {
						break;
					}
					tft->showSettings(brightness / 2.5, player_count, selected);
				}

				if (button != BTN_NONE) {
					tft->setButton(BTN_A, "v");
					tft->setButton(BTN_B, "^");
					tft->setButton(BTN_C, "OK");
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

int changeSetting(u_int8_t min, u_int8_t max, u_int8_t inc, u_int8_t defaultValue, std::function<void(u_int8_t)> updateTFT) {
	tft->setButton(BTN_A, "-");
	tft->setButton(BTN_B, "+");
	tft->setButton(BTN_C, "OK");
	u_int8_t value = defaultValue;
	while (true) {
		u_int8_t button = Buttons::getPressedButton();
		if (button == BTN_A) {
			if (value - inc >= min) {
				value -= inc;
				updateTFT(value);
			}
		} else if (button == BTN_B) {
			if (value + inc <= max) {
				value += inc;
				updateTFT(value);
			}
		} else if (button == BTN_C) {
			return value;
		}
	}
}

void playGame(u_int8_t player_count) {
	int8_t winner;
	game = new Game(tft, led, player_count);

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