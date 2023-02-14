#include "_game.hpp"

Player::Player(short color, Game* game, LED_CONTROLLER* led, Arduino_GFX* gfx) {
	this->color = color;
	this->led = led;
	this->gfx = gfx;
	this->figures_in_base = 4;
	this->selected_figure = nullptr;
	this->game = game;
	for (u_int8_t i = 0; i < 4; i++) {
		this->figures[i] = new Figure(color, this, led);
		this->occupied_goal_positions[i] = false;
	}

	led->setBase(4, color);
}

Player::~Player() {
	for (u_int8_t i = 0; i < 4; i++) {
		delete this->figures[i];
	}
}

void Player::turn() {
	u_int8_t dice_result = roll_dice();

	// If no figure is on the field, if not rolled 6 -> retry
	if (dice_result != 6 && !this->hasFiguresInGame()) {
		for (u_int8_t i = 0; i < 2 && dice_result != 6; i++) {
			dice_result = roll_dice();
		}

		if (dice_result != 6) {
			Serial.println("No figures in game and no 6 -> skipping turn");
			return;
		}
	}

	short field_at_entry_point = this->color * 10 + 1;
	Figure* figure_at_entry_point = getFigureIfAtPosition(field_at_entry_point);

	// If rolled 6 and a figure is in base -> move it to start
	if (dice_result == 6 && figures_in_base > 0) {
		if (figure_at_entry_point == nullptr) {
			for (Figure* f : figures) {
				if (f->isInBase()) {
					this->game->toBaseIfHit(this->color * 10 + 1);
					f->toStart(this->figures_in_base);
					break;
				}
			}

			this->turn();
			return;
		}
	}

	// Find if a particular has to be moved and which
	// TODO: ATM the figure on the starting field has to be moved or the one
	// blocking it from moving -> this is not how it should be done
	short forced_figure_pos = field_at_entry_point;
	while (getFigureIfAtPosition(forced_figure_pos + dice_result) != nullptr) {
		forced_figure_pos = (forced_figure_pos + dice_result) % 40;
	}

	// Sort out figures that can't be moved
	short* pos = this->getPositions();
	short moveable_figures = 0, moveable_figures_pos[4];

	if (figure_at_entry_point != nullptr) {
		moveable_figures = 1;
		moveable_figures_pos[0] = forced_figure_pos;
	} else {
		Serial.println("Positions: ");
		for (u_int8_t i = 0; i < 4; i++) {
			Serial.printf("%d:\n", pos[i]);
			if (pos[i] == 0) continue;

			bool can = true;
			for (u_int8_t j = 0; j < 4; j++) {
				if (i == j) continue;
				Serial.printf("Checking %d against %d", pos[i], pos[j]);

				if (pos[i] < 0) {
					if (pos[j] < 0) {
						if (pos[i] - dice_result == pos[j] ||
							pos[i] - dice_result < -4) {
							can = false;
							Serial.println(" -> no");
							break;
						}
					}
					if (pos[i] - dice_result < -4) {
						can = false;
						Serial.println(" -> no");
						break;
					}
					continue;
				}

				short new_pos = pos[i] + dice_result;
				short field_before_goal =
					this->color * 10 == 0 ? 40 : this->color * 10;
				if (new_pos == pos[j]) {
					can = false;
					Serial.println(" -> no");
					break;
				} else if (pos[i] <= field_before_goal &&
						   new_pos > field_before_goal) {
					short remain = field_before_goal - new_pos;
					if (remain < -4) {
						can = false;
						Serial.println(" -> no");
						break;
					}
				}
			}
			Serial.println(" -> yes");

			if (can) {
				moveable_figures_pos[moveable_figures++] = pos[i];
			}
		}
		Serial.println();
	}

	// Move figure
	if (moveable_figures != 0) {
		this->selected_figure = this->getFigureIfAtPosition(
			FigureSelector::select(this->led, this->color, moveable_figures,
								   moveable_figures_pos));
		if (this->selected_figure == nullptr) {
			Serial.println("Selected figure is null.\nExiting...\n");
			exit(-1);
		}

		this->move(this->selected_figure, dice_result);
	} else {
		Serial.println("No figure can be moved.");
	}

	// Turn again if rolled 6
	if (dice_result == 6) {
		Serial.println("Turn again (rolled 6)...");
		this->turn();
	}
}

bool Player::move(Figure* figure, short offset) {
	Serial.printf("Figure: %d, Offset: %d\n", figure, offset);
	if (figure->getPosition() != 0) {
		Serial.printf("Moving Figure %d by %d\n", figure, offset);
		figure->move(offset);
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
	// TODO: maybe show animation on display

	gfx->setCursor(10, 20);
	gfx->println("Waiting...");

	Serial.println("Rolling dice...\nWaiting for btn3...");
	FigureSelector::waitForConfirm();

	u_int8_t dice_result = rand() % 6 + 1;
	gfx->printf("%d gewuerfelt\n", dice_result);
	Serial.printf("Done waiting...\nRolled %d\n", dice_result);
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