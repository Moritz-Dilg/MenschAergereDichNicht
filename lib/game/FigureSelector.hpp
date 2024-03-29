class FigureSelector {
   public:
	/**
	 * @brief Prompts the user for a figure to select. Syncronous function.
	 * @param led The LED instance
	 * @param positions The positions of the figures
	 *
	 * @returns The index of the selected figure
	 */
	static short select(LED_CONTROLLER* led, u_int8_t color,
						short positions_length, short* positions);

	/**
	 * @brief Waits for button 3 to be pressed
	 */
	static void waitForConfirm();
};