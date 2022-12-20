#include "_game.hpp"
/*
void IRAM_ATTR PRESS_BTN_1() { FigureSelector::pressed_button = 1; }

void IRAM_ATTR PRESS_BTN_2() { FigureSelector::pressed_button = 2; }

void IRAM_ATTR PRESS_BTN_3() { FigureSelector::pressed_button = 3; }*/

Game::Game(Arduino_GFX* gfx) {
	led = new LED_CONTROLLER(LED_BRIGHTNESS);
	led->begin();
	players[0] = new Player(P_BLUE, this, led, gfx);
	players[1] = new Player(P_YELLOW, this, led, gfx);
	players[2] = new Player(P_GREEN, this, led, gfx);
	players[3] = new Player(P_RED, this, led, gfx);
	currentPlayer = 0;
	this->gfx = gfx;

	// TODO: fix buttons
	/*pinMode(PIN_BTN_1, INPUT);
	pinMode(PIN_BTN_2, INPUT);
	pinMode(PIN_BTN_3, INPUT);
	attachInterrupt(PIN_BTN_1, PRESS_BTN_1, FALLING);
	attachInterrupt(PIN_BTN_2, PRESS_BTN_2, FALLING);
	attachInterrupt(PIN_BTN_3, PRESS_BTN_3, FALLING);*/
}

Game::~Game() {
	for (Player* player : players) {
		delete player;
	}
}

void Game::turn() {
	currentPlayer %= 4;
	// TODO: Show turns on TFT
	gfx->fillScreen(BLACK);
	gfx->setCursor(10, 10);
	gfx->setTextColor(WHITE);
	switch (currentPlayer) {
		case P_BLUE:
			gfx->println("Blau ist dran!");
			break;
		case P_YELLOW:
			gfx->println("Gelb ist dran!");
			break;
		case P_GREEN:
			gfx->println("Gruen ist dran!");
			break;
		case P_RED:
			gfx->println("Rot ist dran!");
			break;
		default:
			break;
	}

	players[currentPlayer++]->turn();
}

Figure* Game::getFigureIfAtPosition(const short position) {
	for (Player* p : players) {
		Figure* f = p->getFigureIfAtPosition(position);
		if (f != nullptr) {
			return f;
		}
	}
	return nullptr;
}

bool Game::toBaseIfHit(const short position) {
	for (Player* p : players) {
		if (p->toBaseIfHit(position)) return true;
	}

	return false;
}
