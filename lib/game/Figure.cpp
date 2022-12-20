#include "_game.hpp"

Figure::Figure(short color, Player* player, LED_CONTROLLER* led) {
	this->player = player;
	this->position = 0;
	this->color = color;
	this->led = led;
}

bool Figure::toStart(short& figures_in_base) {
	if (this->position != 0) {
		return false;
	}

	this->position = this->color * 10 + 1;
	figures_in_base--;
	(*this->led).setBase(figures_in_base, this->color);
	(*this->led).setFigureToStart(this->color);
	return true;
}

bool Figure::move(short offset) {
	// Figure in base
	if (this->position == 0) {
		return false;
	}

	short new_position = this->position + offset;

	short field_before_goal = this->color * 10;
	if (field_before_goal <= 0) field_before_goal += 40;
	// Figure in goal
	if (this->position < field_before_goal &&
		new_position >= field_before_goal) {
		short fields_in_goal = new_position - field_before_goal;
		if (fields_in_goal > 4) {
			// TODO: Handle error, if figure doesn't fit in field/goal
			return true;
		}

		// TODO: Check if position in goal is free
		new_position = -fields_in_goal;
		(*this->led).setGoal(-new_position - 1, this->color);
		(*this->led).removeFigureFromField(this->position - 1);
		this->position = new_position;
		return true;
	}

	if (new_position > 40) {
		new_position -= 40;
	}

	this->player->gameToBaseIfHit(new_position);

	(*this->led).moveFigure(this->position - 1, offset, this->color);
	this->position = new_position;
	return true;
}

bool Figure::toBaseIfHit(short position, short& figures_in_base) {
	if (this->position == position) {
		this->position = 0;
		figures_in_base++;
		(*this->led).removeFigureFromField(position - 1);
		(*this->led).setBase(figures_in_base, this->color);
		return true;
	}

	return false;
}

Figure* Figure::getFigureIfAtPosition(short position) {
	if (this->position == position) {
		return this;
	}

	return NULL;
}

short Figure::getPosition() { return this->position; }

bool Figure::isInBase() { return this->position == 0; }