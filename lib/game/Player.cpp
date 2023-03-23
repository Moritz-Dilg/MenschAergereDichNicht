#include "_game.hpp"

Player::Player(short color, Game* game, LED_CONTROLLER* led, TFT_Display* tft) {
	this->color = color;
	this->led = led;
	this->tft = tft;
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

bool Player::turn() {
	tft->resetButtons();
	tft->setButton(BTN_A, "Wuerfeln");

	u_int8_t dice_result = roll_dice();

	// If no figure is on the field, if not rolled 6 -> retry
	if (dice_result != 6 && !this->hasFiguresInGame()) {
		short* dists = find_dists();
		for (u_int8_t i = 0; i < 2 && !contains(dists, dice_result); i++) {
			dice_result = roll_dice();
		}

		if (dice_result != 6) {
			Serial.println("No figures in game and no 6 -> skipping turn");
			return false;
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

			return this->turn();
		}
	}

	// Find if a particular has to be moved and which
	short forced_figure_pos = field_at_entry_point;
	while (getFigureIfAtPosition(forced_figure_pos + dice_result) != nullptr) {
		forced_figure_pos = (forced_figure_pos + dice_result) % 40;
	}

	// Sort out figures that can't be moved
	short* pos = this->getPositions();
	short moveable_figures = 0, moveable_figures_pos[4];

	if (figure_at_entry_point != nullptr && this->figures_in_base > 0) {
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

				// Movement in goal
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
					if (remain == pos[j]) {
						can = false;
						Serial.println(" -> no");
						break;
					}
				}
			}

			if (can) {
				Serial.println(" -> yes");
				moveable_figures_pos[moveable_figures++] = pos[i];
			}
		}
		Serial.println();
	}

	// Move figure
	tft->setButton(BTN_A, "Figur bewegen");
	tft->setButton(BTN_B, "Vorige Figur");
	tft->setButton(BTN_C, "Naechste Figur");
	if (moveable_figures != 0) {
		this->selected_figure = this->getFigureIfAtPosition(
			FigureSelector::select(this->led, this->color, moveable_figures,
								   moveable_figures_pos));
		if (this->selected_figure == nullptr) {
			Serial.println("Selected figure is null.\nExiting...\n");
			exit(-1);  // TODO: remove this
		}

		this->move(this->selected_figure, dice_result);
	} else {
		Serial.println("No figure can be moved.");
	}

	// Turn again if rolled 6
	if (dice_result == 6) {
		Serial.println("Turn again (rolled 6)...");
		return this->turn();
	}
	return hasAllFiguresInGoal();
	// return getFiguresInBase() + hasFiguresInGame() == 3;
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
	// TODO: show instructions on TFT
	tft->setButton(BTN_A, "Wuerfeln");

	Serial.println("Rolling dice...\nWaiting for btn3...");
	FigureSelector::waitForConfirm();

	u_int8_t dice_result = rand() % 6 + 1;

	tft->rollDice(dice_result);
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

short Player::getColor() { return this->color; }

short* Player::find_dists() {
	auto dists = new short[6]{
		(hasFiguresInBase() > 0 ? (short)6 : (short)-1), -1, -1, -1, -1, -1};
	short l = hasFiguresInBase() > 0 ? 1 : 0;

	for (short k = 1; k <= 6 - (hasFiguresInBase() > 0 ? 1 : 0); k++) {
		for (u_int8_t i = 0; i < 4; i++) {
			auto posa = figures[i]->getPosition();
			if (posa == 0) continue;

			bool can = true;
			for (u_int8_t j = 0; j < 4; j++) {
				if (i == j) continue;
				auto posb = figures[j]->getPosition();

				// Movement in goal
				if (posa < 0) {
					if (posb < 0) {
						if (posa - k == posb || posa - k < -4) {
							can = false;
							break;
						}
					}
					if (posa - k < -4) {
						can = false;
						break;
					}
					continue;
				}

				short new_pos = posa + k;
				short field_before_goal =
					this->color * 10 == 0 ? 40 : this->color * 10;
				if (new_pos == posb) {
					can = false;
					break;
				} else if (posa <= field_before_goal &&
						   new_pos > field_before_goal) {
					short remain = field_before_goal - new_pos;
					if (remain < -4) {
						can = false;
						break;
					}
					if (remain == posb) {
						can = false;
						break;
					}
				}
			}

			if (can) {
				dists[l++] = k;
				goto outer;
			}
		}
	outer:
		short a = 0;
	}

	return dists;
}

bool Player::contains(short values[6], short val) {
	for (short i = 0; i < 6; i++) {
		if (values[i] == val) return true;
	}
	return false;
}