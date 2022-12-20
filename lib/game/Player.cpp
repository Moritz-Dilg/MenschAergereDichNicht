#include "_game.hpp"

Player::Player(short color, Game* game, LED_CONTROLLER* led, Arduino_GFX* gfx) {
	this->color = color;
	this->led = led;
	this->gfx = gfx;
	this->figures_in_base = 4;
	this->selected_figure = -1;
	this->game = game;
	for (u_int8_t i = 0; i < 4; i++) {
		this->figures[i] = new Figure(color, this, led);
		this->occupied_goal_positions[i] = false;
	}

	//(*led).setBase(4, color);

	led->setBase(4, color);
}

Player::~Player() {
	for (u_int8_t i = 0; i < 4; i++) {
		delete this->figures[i];
	}
}

void Player::turn() {
	u_int8_t dice_result = roll_dice();

	// If no figure is on the field, try again, if not 6 (max 3 times (including
	// first roll))
	for (u_int8_t i = 0; i < 2 && !this->hasFiguresInGame() &&
						 this->figures_in_base != 0 && dice_result != 6;
		 i++) {
		dice_result = roll_dice();
	}

	short field_at_entry_point = this->color * 10 + 1;
	Figure* figure_at_entry_point = getFigureIfAtPosition(field_at_entry_point);

	if (dice_result == 6 && figures_in_base > 0) {
		// Move figure from base to start
		if (figure_at_entry_point == nullptr) {
			for (Figure* f : figures) {
				if (f->isInBase()) {
					this->game->toBaseIfHit(this->color * 10 + 1);
					f->toStart(this->figures_in_base);
					break;
				}
			}
		}
		// Remove figure from start blocking exit
		else {
			// TODO: Move figure in front of base (or the next one, if it would
			// hit)
		}

		delay(5000);
		// Turn again if rolled 6
		this->turn();
		return;
	}
	// Select figure
	this->selected_figure = 0;
	// FigureSelector::select(this->led, this->getPositions());

	// Move figure
	this->move(this->selected_figure, dice_result);
	delay(5000);

	// Turn again if rolled 6
	if (dice_result == 6) {
		this->turn();
	}
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
		if (this->figures[i]->getFigureIfAtPosition(position) != nullptr) {
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
	// TODO: wait for user to press button
	// TODO: maybe show animation on display
	// TODO: implement rolling multiple dices if a 6 is rolled.
	u_int8_t dice_result = rand() % 6 + 1;
	gfx->setCursor(10, 20);
	gfx->printf("%d gewuerfelt", dice_result);
	return dice_result;
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
	return this->game->getFigureIfAtPosition(position);
}

short Player::getFiguresInBase() { return this->figures_in_base; }

bool Player::gameToBaseIfHit(const short position) {
	return this->game->toBaseIfHit(position);
}