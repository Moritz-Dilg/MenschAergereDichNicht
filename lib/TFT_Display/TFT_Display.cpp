#include "TFT_Display.hpp"

#include <string.h>
Arduino_DataBus *bus;
Arduino_GFX *gfx;

TFT_Display::TFT_Display() {
	bus = new Arduino_ESP32SPI(GFX_DC, GFX_CS, GFX_SCK, GFX_MOSI, GFX_MISO);

	gfx = new Arduino_ST7735(bus, GFX_RST, 3, false, 128, 160, 0, 0, 0, 0,
							 GFX_BGR);

	// TFT
	gfx->begin();
	gfx->fillScreen(BLACK);
	gfx->setTextColor(WHITE);

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

void TFT_Display::showSettings(int brightness, int playerCount, int selected) {
	gfx->fillRect(0, 0, 160, 81, BLACK);
	gfx->setTextColor(WHITE);
	setButton(BTN_A, "^");
	setButton(BTN_B, "v");
	setButton(BTN_C, "OK");

	gfx->fillRect(3, 5 + 18 * selected, 150, 13, DARKGREY);

	if (selected == 0)
		gfx->setTextColor(BLACK);
	else
		gfx->setTextColor(WHITE);
	gfx->setCursor(5, 8);
	gfx->print("LED Helligkeit");
	gfx->setCursor(134, 8);
	gfx->print(brightness);
	gfx->print("%");

	gfx->drawLine(3, 20, 155, 20, DARKGREY);

	if (selected == 1)
		gfx->setTextColor(BLACK);
	else
		gfx->setTextColor(WHITE);
	gfx->setCursor(5, 26);
	gfx->print("Spieleranzahl");
	gfx->setCursor(140, 26);
	gfx->print(playerCount);

	gfx->drawLine(3, 38, 155, 38, DARKGREY);

	if (selected == 2)
		gfx->setTextColor(BLACK);
	else
		gfx->setTextColor(WHITE);
	gfx->setCursor(5, 44);
	gfx->print("Speichern & Beenden");
}

/**
 * @brief Set the text on the top of the TFT
 *
 * @param text Text to print on the top of the TFT
 */
void TFT_Display::setTextTop(const char *text) {
	gfx->setCursor(5, 8);
	gfx->fillRect(0, 0, 160, 20, BLACK);
	gfx->setTextSize(1);
	gfx->setTextColor(WHITE);

	gfx->print(text);

	drawLines();
}

/**
 * @brief Print multiple lines of text in the center left area on the TFT
 *
 * @param text The TextLine to print containing the text and the size
 * @param nLines The number of text lines
 */
void TFT_Display::setTextCenterLeft(TextLine text[], u_int8_t nLines) {
	gfx->fillRect(0, 21, 160 / 2, 60, BLACK);
	gfx->setTextColor(WHITE);

	u_int8_t sumY = 0;
	for (int i = 0; i < nLines; i++) {
		sumY += text[i].size * 8 + 5;
	}

	u_int8_t startY = 21 + (60 - sumY) / 2;

	u_int8_t lastY = startY;
	for (u_int8_t i = 0; i < nLines; i++) {
		lastY = printLine(text[i], 0, 160 / 2, lastY + 5);
	}

	drawLines();
}

/**
 * @brief Print multiple lines of text in the center right area on the TFT
 *
 * @param text The TextLine to print containing the text and the size
 * @param nLines The number of text lines
 */
void TFT_Display::setTextCenterRight(TextLine text[], u_int8_t nLines) {
	gfx->fillRect(160 / 2, 21, 160 / 2, 60, BLACK);
	gfx->setTextColor(WHITE);

	u_int8_t sumY = 0;
	for (int i = 0; i < nLines; i++) {
		sumY += text[i].size * 8 + 5;
	}

	u_int8_t startY = 21 + (60 - sumY) / 2;

	u_int8_t lastY = startY;
	for (u_int8_t i = 0; i < nLines; i++) {
		lastY = printLine(text[i], 160 / 2, 160, lastY + 5);
	}

	drawLines();
}

/**
 * @brief Show which player is currently playing on the TFT
 *
 * @param player Player (Color); use the defines P_BLUE, P_YELLOW, P_GREEN,
 * P_RED
 */
void TFT_Display::setCurrentPlayer(u_int8_t player) {
	switch (player) {
		case P_BLUE:
			setTextTop("Spieler blau am Zug!");
			gfx->fillCircle(150, 10, 5, BLUE);
			break;
		case P_YELLOW:
			setTextTop("Spieler gelb am Zug!");
			gfx->fillCircle(150, 10, 5, YELLOW);
			break;
		case P_GREEN:
			setTextTop("Spieler gruen am Zug!");
			gfx->fillCircle(150, 10, 5, GREEN);
			break;
		case P_RED:
			setTextTop("Spieler rot am Zug!");
			gfx->fillCircle(150, 10, 5, RED);
			break;

		default:
			break;
	}
}

/**
 * @brief Show the result of the dice roll on the TFT
 *
 * @param result Result of the dice roll to display
 */
void TFT_Display::rollDice(u_int8_t result) {
	char buffer[10];
	itoa(result, buffer, 10);
	TextLine text[] = {
		{buffer, 3},
		{"gewuerfelt", 1},
	};

	setTextCenterLeft(text, 2);

	drawLines();
}

void TFT_Display::clearDice() {
	gfx->fillRect(0, 21, 160 / 2, 60, BLACK);
	drawLines();
}

/**
 * @brief Show the next instruction on the TFT
 *
 * @param instruction Instruction to display
 */
void TFT_Display::setNextInstruction(const char *instruction) {
	// TODO: add linebreaks
	setTextCenterRight(new TextLine({instruction, 1}), 1);

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
	gfx->setTextColor(WHITE);

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

	drawButtonLines();
}

/**
 * @brief Print a line of text in the center of the area specified on the TFT
 *
 * @param line The TextLine to print containing the text and the size
 * @param startX The left border of the text-area
 * @param startY The top border of the text-area
 * @param stopX The right border of the text-area
 * @param stopY The bottom border of the text-area
 * @return u_int8_t The y-coordinate of the bottom of the text
 */
u_int8_t TFT_Display::printLine(TextLine line, u_int8_t startX, u_int8_t stopX,
								u_int8_t y) {
	u_int8_t length = strlen(line.text);
	u_int8_t lengthPX = length * line.size * 6;

	u_int8_t x = startX + (stopX - startX - lengthPX) / 2;

	gfx->setCursor(x, y);
	gfx->setTextColor(WHITE);
	gfx->setTextSize(line.size);
	gfx->print(line.text);

	return y + line.size * 8;
}

void TFT_Display::resetButtons() {
	gfx->fillRect(0, 80, 160, 48, BLACK);
	gfx->setTextColor(WHITE);

	gfx->setTextSize(2);
	gfx->setCursor(20, 88);
	gfx->print("A");
	gfx->setCursor(74, 88);
	gfx->print("B");
	gfx->setCursor(128, 88);
	gfx->print("C");
	gfx->setTextSize(1);

	drawButtonLines();
}

void TFT_Display::clearScreen() {
	gfx->fillScreen(BLACK);
	resetButtons();
}

void TFT_Display::drawLines() {
	gfx->drawLine(0, 20, 160, 20, WHITE);
	gfx->drawLine(160 / 2, 20, 160 / 2, 80, WHITE);

	drawButtonLines();
}

void TFT_Display::drawButtonLines() {
	gfx->drawLine(0, 80, 160, 80, WHITE);

	gfx->drawLine(52, 80, 52, 128, WHITE);
	gfx->drawLine(105, 80, 105, 128, WHITE);
}

TFT_Display::~TFT_Display() {
	gfx->fillScreen(BLACK);

	delete gfx;
	delete bus;
}
