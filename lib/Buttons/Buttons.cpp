#include "Buttons.hpp"

void IRAM_ATTR PRESS_BTN_A() { Buttons::pressed_button = BTN_A; }

void IRAM_ATTR PRESS_BTN_B() { Buttons::pressed_button = BTN_B; }

void IRAM_ATTR PRESS_BTN_C() { Buttons::pressed_button = BTN_C; }

Buttons::Buttons() {
	pinMode(PIN_BTN_A, INPUT);
	pinMode(PIN_BTN_B, INPUT);
	pinMode(PIN_BTN_C, INPUT);
	attachInterrupt(PIN_BTN_A, PRESS_BTN_A, FALLING);
	attachInterrupt(PIN_BTN_B, PRESS_BTN_B, FALLING);
	attachInterrupt(PIN_BTN_C, PRESS_BTN_C, FALLING);
}

/**
 * @brief Get the Pressed Button
 *
 * @return u_int8_t The pressed Button; compare against BTN_A, BTN_B and BTN_C;
 * With BTN_NONE defined as no button pressed.
 */
u_int8_t Buttons::getPressedButton() {
	u_int8_t button = pressed_button;
	pressed_button = BTN_NONE;
	return button;
}

Buttons::~Buttons() {
	detachInterrupt(PIN_BTN_A);
	detachInterrupt(PIN_BTN_B);
	detachInterrupt(PIN_BTN_C);
}
