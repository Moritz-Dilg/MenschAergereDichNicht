#include "LED.hpp"

class FigureSelector {
   public:
	/**
	 * @brief The button pressed by the user. Updated by an interrupt routine.
	 */
	static u_int8_t pressed_button;

	/**
	 * @brief Prompts the user for a figure to select. Syncronous function.
	 * @param led The LED instance
	 * @param positions The positions of the figures
	 *
	 * @returns The index of the selected figure
	 */
	static short select(LED* led, short positions[4]);
};