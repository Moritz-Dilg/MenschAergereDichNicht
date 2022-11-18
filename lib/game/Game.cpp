#include "Game.hpp"

#include "FigureSelector.hpp"

Game::Game() {
	led = new LED(LED_BRIGHTNESS);
	players[0] = new Player(BLUE, this, led);
	players[1] = new Player(YELLOW, this, led);
	players[2] = new Player(GREEN, this, led);
	players[3] = new Player(RED, this, led);
	currentPlayer = 0;

	pinMode(PIN_BTN_1, INPUT);
	pinMode(PIN_BTN_2, INPUT);
	pinMode(PIN_BTN_3, INPUT);
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

Figure* Game::getFigureIfAtPosition(const short position) {
	for (Player* p : players) {
		auto positions = p->getPositions();
		for (short i = 0; i < 4; i++) {
			Figure* f = p->getFigureIfAtPosition(position);
			if (f != nullptr) {
				return f;
			}
		}
	}
	return nullptr;
}

void IRAM_ATTR PRESS_BTN_1() { FigureSelector::pressed_button = 1; }

void IRAM_ATTR PRESS_BTN_2() { FigureSelector::pressed_button = 2; }

void IRAM_ATTR PRESS_BTN_3() { FigureSelector::pressed_button = 3; }