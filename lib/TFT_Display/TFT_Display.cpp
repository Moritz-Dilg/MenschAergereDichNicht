#include "TFT_Display.hpp"

#include <string.h>
Arduino_DataBus *bus;
Arduino_GFX *gfx;

// TODO: clear Dice result on new player
// TODO: Set button asignments in game

TFT_Display::TFT_Display(short brightness) {
	bus = new Arduino_ESP32SPI(GFX_DC, GFX_CS, GFX_SCK, GFX_MOSI, GFX_MISO);

	gfx = new Arduino_ST7735(bus, GFX_RST, 3, false, 128, 160, 0, 0, 0, 0,
							 GFX_BGR);

	// TFT
	gfx->begin();
	gfx->fillScreen(BLACK);

	gfx->setTextSize(2);
	gfx->setCursor(20, 88);
	gfx->print("A");
	gfx->setCursor(74, 88);
	gfx->print("B");
	gfx->setCursor(128, 88);
	gfx->print("C");
	gfx->setTextSize(1);

	drawLines();
}

void TFT_Display::showSettings() {
	// TODO: implement
}

/**
 * @brief Show which player is currently playing on the TFT
 *
 * @param player Player (Color); use the defines P_BLUE, P_YELLOW, P_GREEN,
 * P_RED
 */
void TFT_Display::setCurrentPlayer(u_int8_t player) {
	gfx->setCursor(5, 8);
	gfx->fillRect(0, 0, 160, 20, BLACK);
	gfx->setTextSize(1);

	gfx->print("Spieler ");

	switch (player) {
		case P_BLUE:
			gfx->print("blau");
			gfx->fillCircle(150, 10, 5, BLUE);
			break;
		case P_YELLOW:
			gfx->print("gelb");
			gfx->fillCircle(150, 10, 5, YELLOW);
			break;
		case P_GREEN:
			gfx->print("gruen");
			gfx->fillCircle(150, 10, 5, GREEN);
			break;
		case P_RED:
			gfx->print("rot");
			gfx->fillCircle(150, 10, 5, RED);
			break;

		default:
			break;
	}
	gfx->print(" am Zug!");

	drawLines();
}

/**
 * @brief Show the result of the dice roll on the TFT
 *
 * @param result Result of the dice roll to display
 */
void TFT_Display::rollDice(u_int8_t result) {
	gfx->fillRect(0, 21, 160 / 2, 38 + 5, BLACK);

	gfx->setCursor(32, 35);
	gfx->setTextSize(3);
	gfx->print(result);

	gfx->setCursor(10, 64);
	gfx->setTextSize(1);
	gfx->print("gewuerfelt");

	drawLines();
}

/**
 * @brief Show the next instruction on the TFT
 *
 * @param instruction Instruction to display
 */
void TFT_Display::setNextInstruction(const char *instruction) {
	// TODO: add linebreaks
	gfx->fillRect(160 / 2, 21, 160 / 2, 38 + 5, BLACK);

	gfx->setCursor(160 / 2 + 10, 30);
	gfx->setTextSize(1);
	gfx->print(instruction);

	drawLines();
}

/**
 * @brief Set the text of a button
 *
 * @param button Button to set the text of, use the defines BTN_A, BTN_B, BTN_C
 * @param text Functionality description of the button
 */
void TFT_Display::setButton(u_int8_t button, const char *text) {
	u_int8_t offset = (button - 1) * 54;
	gfx->fillRect(0 + offset, 80, 52, 48, BLACK);

	gfx->setTextSize(2);
	gfx->setCursor(20 + offset, 88);

	switch (button) {
		case BTN_A:
			gfx->print("A");
			break;
		case BTN_B:
			gfx->print("B");
			break;
		case BTN_C:
			gfx->print("C");
			break;
	}

	gfx->setTextSize(1);
	gfx->setCursor(2 + offset, 105);

	char *textArray[5] = {};
	char str[100];
	strcpy(str, text);
	char *ptr = strtok(str, " ");

	for (u_int8_t i; ptr != NULL; i++) {
		textArray[i] = ptr;
		ptr = strtok(NULL, " ");
	}

	for (u_int8_t i = 0; i < 2; i++) {
		if (textArray[i] == NULL) break;
		int len = strlen(textArray[i]);
		gfx->setCursor((8 - len) / (float)2 * 6 + 2 + offset, 105 + i * 10);
		gfx->print(textArray[i]);
	}

	drawLines();
}

void TFT_Display::resetButtons() {
	gfx->fillRect(0, 80, 160, 48, BLACK);

	gfx->setTextSize(2);
	gfx->setCursor(20, 88);
	gfx->print("A");
	gfx->setCursor(74, 88);
	gfx->print("B");
	gfx->setCursor(128, 88);
	gfx->print("C");
	gfx->setTextSize(1);

	drawLines();
}

void TFT_Display::drawLines() {
	gfx->drawLine(0, 20, 160, 20, WHITE);
	gfx->drawLine(0, 80, 160, 80, WHITE);
	gfx->drawLine(160 / 2, 20, 160 / 2, 80, WHITE);

	gfx->drawLine(52, 80, 52, 128, WHITE);
	gfx->drawLine(105, 80, 105, 128, WHITE);
}

TFT_Display::~TFT_Display() {
	delete gfx;
	delete bus;
}
