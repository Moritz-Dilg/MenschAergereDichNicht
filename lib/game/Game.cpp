#include "Game.hpp"

Game::Game() {
	led = new LED(LED_BRIGHTNESS);
	players[0] = new Player(BLUE, led);
	players[1] = new Player(YELLOW, led);
	players[2] = new Player(GREEN, led);
	players[3] = new Player(RED, led);
	currentPlayer = 0;
}

Game::~Game() {
	for (Player* player : players) {
		delete player;
	}
	delete players;
}

void Game::turn() { players[currentPlayer++]->turn(); }