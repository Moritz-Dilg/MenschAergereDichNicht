#include "_game.hpp"

void IRAM_ATTR PRESS_BTN_A() { FigureSelector::pressed_button = BTN_A; }

void IRAM_ATTR PRESS_BTN_B() { FigureSelector::pressed_button = BTN_B; }

void IRAM_ATTR PRESS_BTN_C() { FigureSelector::pressed_button = BTN_C; }

Game::Game(Arduino_GFX* gfx) {
	led = new LED_CONTROLLER(LED_BRIGHTNESS);
	led->begin();
	players[0] = new Player(P_BLUE, this, led, gfx);
	players[1] = new Player(P_YELLOW, this, led, gfx);
	players[2] = new Player(P_GREEN, this, led, gfx);
	players[3] = new Player(P_RED, this, led, gfx);
	currentPlayer = 0;
	this->gfx = gfx;

	pinMode(PIN_BTN_A, INPUT);
	pinMode(PIN_BTN_B, INPUT);
	pinMode(PIN_BTN_C, INPUT);
	attachInterrupt(PIN_BTN_A, PRESS_BTN_A, FALLING);
	attachInterrupt(PIN_BTN_B, PRESS_BTN_B, FALLING);
	attachInterrupt(PIN_BTN_C, PRESS_BTN_C, FALLING);
}

Game::~Game() {
	for (Player* player : players) {
		delete player;
	}
}

void Game::turn() {
	Serial.println("turn\n\n\n");

	currentPlayer %= 4;
	// TODO: Show turns on TFT
	gfx->fillScreen(BLACK);
	gfx->setCursor(10, 10);
	gfx->setTextColor(WHITE);
	switch (currentPlayer) {
		case P_BLUE:
			Serial.println("Blau ist dran!");
			gfx->println("Blau ist dran!");
			break;
		case P_YELLOW:
			Serial.println("Gelb ist dran!");
			gfx->println("Gelb ist dran!");
			break;
		case P_GREEN:
			Serial.println("Gruen ist dran!");
			gfx->println("Gruen ist dran!");
			break;
		case P_RED:
			Serial.println("Rot ist dran!");
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
