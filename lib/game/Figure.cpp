#include "Figure.hpp"

Figure::Figure(short color, LED& led) : color(color), led(led) {
    this->position = 0;
    this->color = color;
    this->led = led;
}

bool Figure::toStart(short& players_in_base) {
    if (this->position != 0) {
        return false;
    }

    this->position = this->color * 10 + 1;
    players_in_base--;
    this->led.setBase(players_in_base, this->color);
    this->led.setFigureToStart(this->color);
    return true;
}

bool Figure::move(short offset) {
    // Figure in base
    if (this->position == 0) {
        return false;
    }

    short new_position = this->position + offset;
    if (new_position > 40) {
        new_position -= 40;
    }

    short field_before_goal = this->color * 10 - 1;
    if (field_before_goal < 0) field_before_goal += 40;
    // Figure in goal
    if (this->position < field_before_goal && new_position >= field_before_goal) {
        short fields_in_goal = new_position - field_before_goal;
        if (fields_in_goal > 4) {
            return true;
        }

        // TODO: Check if position in goal is free
        new_position = -fields_in_goal;
        this->led.setGoal(new_position, this->color);
        this->led.removeFigureFromField(this->position - 1);
        this->position = new_position;
        return true;
    }

    this->position = new_position;
    this->led.moveFigure(this->position, offset, this->color);
    return true;
}

bool Figure::toBaseIfHit(short position, short& players_in_base) {
    if (this->position == position) {
        this->position = 0;
        players_in_base++;
        this->led.removeFigureFromField(position - 1);
        this->led.setBase(players_in_base, this->color);
        return true;
    }

    return false;
}

void Figure::select() {
}

void Figure::deselect() {
}

Figure* Figure::getFigureIfAtPosition(short position) {
    if (this->position == position) {
        return this;
    }

    return NULL;
}
