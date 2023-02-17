#include "_game.hpp"

void IRAM_ATTR PRESS_BTN_A() { FigureSelector::pressed_button = BTN_A; }

void IRAM_ATTR PRESS_BTN_B() { FigureSelector::pressed_button = BTN_B; }

void IRAM_ATTR PRESS_BTN_C() { FigureSelector::pressed_button = BTN_C; }

Game::Game(TFT_Display* tft, short player_count) {
	if (player_count < 2 || player_count > 4) {
		Serial.println("Invalid player count!");
		exit(-1);
		return;
	}

	led = new LED_CONTROLLER(LED_BRIGHTNESS);
	led->begin();

	this->player_count = player_count;
	players[0] = new Player(P_BLUE, this, led, tft);
	if (player_count == 2) {
		players[1] = new Player(P_GREEN, this, led, tft);
	} else if (player_count == 3) {
		players[1] = new Player(P_YELLOW, this, led, tft);
		players[2] = new Player(P_GREEN, this, led, tft);
	} else if (player_count == 4) {
		players[1] = new Player(P_YELLOW, this, led, tft);
		players[2] = new Player(P_GREEN, this, led, tft);
		players[3] = new Player(P_RED, this, led, tft);
	}
	currentPlayer = 0;
	this->tft = tft;

	pinMode(PIN_BTN_A, INPUT);
	pinMode(PIN_BTN_B, INPUT);
	pinMode(PIN_BTN_C, INPUT);
	attachInterrupt(PIN_BTN_A, PRESS_BTN_A, FALLING);
	attachInterrupt(PIN_BTN_B, PRESS_BTN_B, FALLING);
	attachInterrupt(PIN_BTN_C, PRESS_BTN_C, FALLING);
}

Game::~Game() {
	for (short i = 0; i < player_count; i++) {
		delete players[i];
	}
}

void Game::turn() {
	Serial.println("turn\n\n\n");

	currentPlayer = currentPlayer % player_count;
	if (player_count == 2) {
		tft->setCurrentPlayer(currentPlayer == P_BLUE ? P_BLUE : P_GREEN);
	} else {
		tft->setCurrentPlayer(currentPlayer);
	}
	switch (currentPlayer) {
		case P_BLUE:
			Serial.println("Blau ist dran!");
			break;
		case P_YELLOW:
			Serial.println("Gelb ist dran!");
			break;
		case P_GREEN:
			Serial.println("Gruen ist dran!");
			break;
		case P_RED:
			Serial.println("Rot ist dran!");
			break;
		default:
			break;
	}

	players[currentPlayer]->turn();
	currentPlayer++;
}

Figure* Game::getFigureIfAtPosition(const short position) {
	for (short i = 0; i < player_count; i++) {
		Figure* f = players[i]->getFigureIfAtPosition(position);
		if (f != nullptr) {
			return f;
		}
	}
	return nullptr;
}

bool Game::toBaseIfHit(const short position) {
	for (short i = 0; i < player_count; i++) {
		if (players[i]->toBaseIfHit(position)) return true;
	}

	return false;
}
