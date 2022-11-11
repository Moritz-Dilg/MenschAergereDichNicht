#include "Game.hpp"

#include "FigureSelector.hpp"

Game::Game() {
	led = new LED(LED_BRIGHTNESS);
	players[0] = new Player(BLUE, led);
	players[1] = new Player(YELLOW, led);
	players[2] = new Player(GREEN, led);
	players[3] = new Player(RED, led);
	currentPlayer = 0;

	pinMode(PIN_BTN_1, INPUT_PULLUP);
	pinMode(PIN_BTN_2, INPUT_PULLUP);
	pinMode(PIN_BTN_3, INPUT_PULLUP);
	attachInterrupt(PIN_BTN_1, PRESS_BTN_1, FALLING);
	attachInterrupt(PIN_BTN_2, PRESS_BTN_2, FALLING);
	attachInterrupt(PIN_BTN_3, PRESS_BTN_3, FALLING);
}

Game::~Game() {
	for (Player* player : players) {
		delete player;
	}
	delete players;
}

void Game::turn() { players[currentPlayer++]->turn(); }

void IRAM_ATTR PRESS_BTN_1() { FigureSelector::pressed_button = 1; }

void IRAM_ATTR PRESS_BTN_2() { FigureSelector::pressed_button = 2; }

void IRAM_ATTR PRESS_BTN_3() { FigureSelector::pressed_button = 3; }