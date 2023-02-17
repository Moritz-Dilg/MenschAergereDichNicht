#include <Arduino_GFX_Library.h>

#include "../types.hpp"

class TFT_Display {
   public:
	TFT_Display(short brightness);
	~TFT_Display();

	void showSettings();
	void setCurrentPlayer(u_int8_t player);
	void rollDice(u_int8_t result);
	void setNextInstruction(const char* instruction);
	void setButton(u_int8_t button, const char* text);
	void resetButtons();

   private:
	void drawLines();
};