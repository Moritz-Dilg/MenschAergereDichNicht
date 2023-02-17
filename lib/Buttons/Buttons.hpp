
#include "../types.hpp"
#include "Arduino.h"

class Buttons {
   public:
	Buttons();
	~Buttons();
	static u_int8_t getPressedButton();

	/**
	 * @brief The button pressed by the user. Updated by an interrupt routine.
	 * With BTN_NONE defined as no button pressed.
	 */
	static inline volatile u_int8_t pressed_button = BTN_NONE;
};
