#include <Arduino_GFX_Library.h>

#include "../types.hpp"

struct TextLine {
	const char* text;
	u_int8_t size;
};

class TFT_Display {
   public:
	TFT_Display();
	~TFT_Display();

	void showSettings(int brightness, int playerCount, int selected);
	void setTextTop(const char* text);
	void setTextCenterLeft(TextLine text[], u_int8_t nLines);
	void setTextCenterRight(TextLine text[], u_int8_t nLines);
	void setCurrentPlayer(u_int8_t player);
	void rollDice(u_int8_t result);
	void clearDice();
	void setNextInstruction(const char* instruction);
	void setButton(u_int8_t button, const char* text);
	void resetButtons();
	void clearScreen();

   private:
	void drawLines();
	void drawButtonLines();
	u_int8_t printLine(TextLine line, u_int8_t startX, u_int8_t stopX,
					   u_int8_t y);
};