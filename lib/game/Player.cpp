#include "Player.hpp"

Player::Player(short color, Game* game, LED* led) {
	this->color = color;
	this->led = led;
	this->figures_in_base = 4;
	this->selected_figure = -1;
	this->game = game;
	for (u_int8_t i = 0; i < 4; i++) {
		this->figures[i] = new Figure(color, this, led);
		this->occupied_goal_positions[i] = false;
	}

	(*led).setBase(4, color);
}

Player::~Player() {
	for (u_int8_t i = 0; i < 4; i++) {
		delete this->figures[i];
	}
	delete this->figures;
}

void Player::turn() {
	u_int8_t dice_result = roll_dice();

	// Select figure
	this->selected_figure =
		FigureSelector::select(this->led, this->getPositions());

	// Move figure
	this->move(this->selected_figure, dice_result);
}

bool Player::move(short figure, short offset) {
	if (this->figures[figure]->getPosition() != 0) {
		this->figures[figure]->move(offset);
		return true;
	}
	return false;
}

bool Player::isGoalPosFree(short position) {
	return !this->occupied_goal_positions[position];
}

short Player::hasFiguresInGame() {
	u_int8_t c = 0;
	for (u_int8_t i = 0; i < 4; i++) {
		if (this->isGoalPosFree(i)) {
			c++;
		}
	}
	return c - this->hasFiguresInBase();
}

short Player::hasFiguresInBase() { return this->figures_in_base; }

bool Player::hasAllFiguresInGoal() {
	return this->hasFiguresInBase() == 0 && this->hasFiguresInGame() == 0;
}

bool Player::toBaseIfHit(short position) {
	for (u_int8_t i = 0; i < 4; i++) {
		if (this->figures[i]->getFigureIfAtPosition(position) != NULL) {
			this->figures[i]->toBaseIfHit(position, this->figures_in_base);
			return true;
		}
	}
	return false;
}

short* Player::getPositions() {
	short* positions = new short[4];
	for (u_int8_t i = 0; i < 4; i++) {
		positions[i] = this->figures[i]->getPosition();
	}
	return positions;
}

u_int8_t Player::roll_dice() {
	// TODO: maybe show animation on display
	// TODO: implement rolling multiple dices if a 6 is rolled.
	return rand() % 6 + 1;
}

Figure* Player::getFigureIfAtPosition(const short position) {
	for (int i = 0; i < 4; i++) {
		if (this->figures[i]->getPosition() == position) {
			return this->figures[i];
		}
	}
	return nullptr;
}

Figure* Player::getOpposingFigure(const short position) {
	return game->getFigureIfAtPosition(position);
}