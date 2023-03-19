#include "_game.hpp"

Game::Game(TFT_Display* tft, LED_CONTROLLER* led, short player_count) {
	if (player_count < 2 || player_count > 4) {
		Serial.println("Invalid player count!");
		exit(-1);
		return;
	}

	this->led = led;
	this->led->clearAll();

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
}

Game::~Game() {
	for (short i = 0; i < player_count; i++) {
		delete players[i];
	}
}

int Game::turn() {
	Serial.println("turn\n\n\n");

	currentPlayer %= player_count;
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
	tft->clearDice();

	if (players[currentPlayer]->turn()) {
		Serial.println("GAME OVER!");
		return players[currentPlayer]->getColor();
	}
	currentPlayer++;
	return -1;
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
