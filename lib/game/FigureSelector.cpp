#include "_game.hpp"

short FigureSelector::select(LED_CONTROLLER* led, u_int8_t color,
							 short positions_length, short* positions) {
	Serial.printf("entering selector\n\tlen: %d\n\tpos: %d, %d, %d, %d\n",
				  positions_length, positions[0], positions[1], positions[2],
				  positions[3]);

	u_int8_t selected = 0;
	u_int8_t prevSelected = 0;
	u_int64_t last_update = 0;
	bool toggle = false;

	Serial.println(
		"----------------------\nSEMI Waiting for "
		"confirm...\n----------------------");

	while (pressed_button != 3) {
		u_int64_t current_time = millis();
		if (current_time - last_update >= FIGURE_SELECT_TOGGLE_TIME) {
			if (selected != prevSelected) {
				Serial.println("switching\n");
				led->setFigure(positions[prevSelected], color);
				led->clearFigure(positions[selected], color);
				prevSelected = selected;
				toggle = false;
			} else {
				if (toggle) {
					led->clearFigure(positions[selected], color);
				} else {
					led->setFigure(positions[selected], color);
				}
				toggle = !toggle;
			}

			last_update = current_time;
		}

		if (pressed_button == 1) {
			pressed_button = 0;
			Serial.println("b1");
			prevSelected = selected;
			selected = selected + 1 > positions_length - 1 ? 0 : selected + 1;
			Serial.printf("Selected: %d\n", selected);
			delay(50);
		} else if (pressed_button == 2) {
			pressed_button = 0;
			Serial.println("b2");
			prevSelected = selected;
			selected = selected - 1 < 0 ? positions_length - 1 : selected - 1;
			Serial.printf("Selected: %d\n", selected);
			delay(50);
		}
	}

	pressed_button = 0;
	Serial.printf("exiting selector: %d\n", selected);
	return positions[selected];
}

void FigureSelector::waitForConfirm() {
	Serial.println(
		"----------------------\nWaiting for "
		"confirm...\n----------------------");
	while (pressed_button != 3)
		;
	pressed_button = 0;
}