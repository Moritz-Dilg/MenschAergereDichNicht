#include "FigureSelector.hpp"

#include <chrono>

short FigureSelector::select(LED* led, short positions[4]) {
	u_int8_t selected = 0;
	u_int64_t last_update = 0;

	while (true) {
		u_int64_t current_time =
			std::chrono::duration_cast<std::chrono::milliseconds>(
				std::chrono::system_clock::now().time_since_epoch())
				.count();
		if (current_time - last_update >= FIGURE_SELECT_TOGGLE_TIME) {
			// TODO: toggle selected LED

			last_update = current_time;
		}

		switch (FigureSelector::pressed_button) {
			case 1:
				selected = selected + 1 > 3 ? 0 : selected + 1;
				break;
			case 2:
				selected = selected - 1 < 0 ? 3 : selected - 1;
				break;
			case 3:
				return selected;
			default:
				break;
		}
		FigureSelector::pressed_button = 0;
	}
}