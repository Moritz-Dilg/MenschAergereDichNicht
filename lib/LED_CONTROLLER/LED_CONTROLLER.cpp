#include <LED_CONTROLLER.hpp>

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel fieldStrip =
	Adafruit_NeoPixel(N_PIXELS_FIELD, LED_PIN_FIELD, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel goalStrip =
	Adafruit_NeoPixel(N_PIXELS_GOAL, LED_PIN_GOAL, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel baseStrip =
	Adafruit_NeoPixel(N_PIXELS_BASE, LED_PIN_BASE, NEO_GRB + NEO_KHZ800);

/**
 * @brief Construct a new LED_CONTROLLER::LED object
 *
 * @param brightness Brightness of the LEDs: 1 - 255
 */
LED_CONTROLLER::LED_CONTROLLER(uint8_t brightness) {
	this->brightness = brightness;
	clearAll();
}

// Initialize LED
void LED_CONTROLLER::begin() {
	fieldStrip.begin();
	fieldStrip.setBrightness(this->brightness);
	fieldStrip.show();	// Initialize all pixels to 'off'

	goalStrip.begin();
	goalStrip.setBrightness(this->brightness);
	goalStrip.show();  // Initialize all pixels to 'off'

	baseStrip.begin();
	baseStrip.setBrightness(this->brightness);
	baseStrip.show();  // Initialize all pixels to 'off'
}

/**
 * @brief Set the base LEDs
 *
 * @param n Number of figures in the base
 * @param color Color to set, use the defines P_BLUE, P_YELLOW, P_GREEN, P_RED
 */
void LED_CONTROLLER::setBase(u_int8_t n, u_int8_t color) {
	char ledOrder[16] = {0, 1, 3, 2, 7, 6, 4, 5, 9, 8, 10, 11, 13, 14, 12, 15};

	if (!verifyNFigures(n)) return;
	clearSection(color, baseStrip);

	for (u_int8_t i = color * 4; i < color * 4 + n; i++) {
		switch (color) {
			case P_BLUE:
				baseStrip.setPixelColor(ledOrder[i],
										baseStrip.Color(0, 0, 255));
				break;

			case P_YELLOW:
				baseStrip.setPixelColor(ledOrder[i],
										baseStrip.Color(255, 100, 0));
				break;

			case P_GREEN:
				baseStrip.setPixelColor(ledOrder[i],
										baseStrip.Color(0, 255, 0));
				break;

			case P_RED:
				baseStrip.setPixelColor(ledOrder[i],
										baseStrip.Color(255, 0, 0));
				break;

			default:
				break;
		}
	}
	baseStrip.show();
}

/**
 * @brief Set the goal LEDs
 *
 * @param n Position of figure in goal sarting at 0
 * @param color Color to set, use the defines P_BLUE, P_YELLOW, P_GREEN, P_RED
 * @param from If figure moved in goal -> previous position (leave blank if
 * figure just arrived in goal)
 */
void LED_CONTROLLER::setGoal(u_int8_t n, u_int8_t color, int8_t from) {
	if (!verifyNFigures(n)) return;

	switch (color) {
		case P_BLUE:
			if (from != -1)
				goalStrip.setPixelColor(color * 4 + from,
										goalStrip.Color(0, 0, 0));
			goalStrip.setPixelColor(color * 4 + n, goalStrip.Color(0, 0, 255));
			break;

		case P_YELLOW:
			if (from != -1)
				goalStrip.setPixelColor(color * 4 + (3 - from),
										goalStrip.Color(0, 0, 0));
			goalStrip.setPixelColor(color * 4 + (3 - n),
									goalStrip.Color(255, 100, 0));
			break;

		case P_GREEN:
			if (from != -1)
				goalStrip.setPixelColor(color * 4 + (3 - from),
										goalStrip.Color(0, 0, 0));
			goalStrip.setPixelColor(color * 4 + (3 - n),
									goalStrip.Color(0, 255, 0));
			break;

		case P_RED:
			if (from != -1)
				goalStrip.setPixelColor(color * 4 + (3 - from),
										goalStrip.Color(0, 0, 0));
			goalStrip.setPixelColor(color * 4 + (3 - n),
									goalStrip.Color(255, 0, 0));
			break;

		default:
			break;
	}

	goalStrip.show();
}

/**
 * @brief Set first LED of the field outside of the base
 *
 * @param color Color to set, use the defines P_BLUE, P_YELLOW, P_GREEN, P_RED
 */
void LED_CONTROLLER::setFigureToStart(u_int8_t color) {
	switch (color) {
		case P_BLUE:
			fieldStrip.setPixelColor(0, fieldStrip.Color(0, 0, 255));
			break;

		case P_YELLOW:
			fieldStrip.setPixelColor(10, fieldStrip.Color(255, 100, 0));
			break;

		case P_GREEN:
			fieldStrip.setPixelColor(20, fieldStrip.Color(0, 255, 0));
			break;

		case P_RED:
			fieldStrip.setPixelColor(30, fieldStrip.Color(255, 0, 0));
			break;

		default:
			break;
	}
	fieldStrip.show();
}

/**
 * @brief Remove figure at given position from field
 *
 * @param position Position of figure to remove starting at 0
 */
void LED_CONTROLLER::removeFigureFromField(u_int8_t position) {
	Serial.println("Remove figure from field");
	Serial.println(position);
	fieldStrip.setPixelColor(position, fieldStrip.Color(0, 0, 0));
	fieldStrip.show();
}

/**
 * @brief Move figure from Index i (starting at 0) n figures forward
 *
 * @param i Index of the LED to move starting at 0
 * @param n Number of fields to move forward
 * @param color Color to set, use the defines P_BLUE, P_YELLOW, P_GREEN, P_RED
 */
void LED_CONTROLLER::moveFigure(u_int8_t i, u_int8_t n, u_int8_t color) {
	fieldStrip.setPixelColor(i, fieldStrip.Color(0, 0, 0));
	u_int8_t new_position = i + n;
	if (new_position > 39) new_position -= 40;
	switch (color) {
		case P_BLUE:
			fieldStrip.setPixelColor(new_position, fieldStrip.Color(0, 0, 255));
			break;

		case P_YELLOW:
			fieldStrip.setPixelColor(new_position,
									 fieldStrip.Color(255, 100, 0));
			break;

		case P_GREEN:
			fieldStrip.setPixelColor(new_position, fieldStrip.Color(0, 255, 0));
			break;

		case P_RED:
			fieldStrip.setPixelColor(new_position, fieldStrip.Color(255, 0, 0));
			break;

		default:
			break;
	}
	fieldStrip.show();
}

/**
 * @brief Set a figure at a given position ()
 *
 * @param i Position of figure (field: 1...40, goal: -1...-4)
 * @param color Color to set, use the defines P_BLUE, P_YELLOW, P_GREEN, P_RED
 */
void LED_CONTROLLER::setFigure(int8_t i, u_int8_t color) {
	if (i <= 0) {
		switch (color) {
			case P_BLUE:
				goalStrip.setPixelColor(color * 4 - i - 1,
										goalStrip.Color(0, 0, 255));
				break;

			case P_YELLOW:
				goalStrip.setPixelColor(color * 4 + (4 + i),
										goalStrip.Color(255, 100, 0));
				break;

			case P_GREEN:
				goalStrip.setPixelColor(color * 4 + (4 + i),
										goalStrip.Color(0, 255, 0));
				break;

			case P_RED:
				goalStrip.setPixelColor(color * 4 + (4 + i),
										goalStrip.Color(255, 0, 0));
				break;

			default:
				break;
		}
		goalStrip.show();
	} else {
		switch (color) {
			case P_BLUE:
				fieldStrip.setPixelColor(i - 1, fieldStrip.Color(0, 0, 255));
				break;

			case P_YELLOW:
				fieldStrip.setPixelColor(i - 1, fieldStrip.Color(255, 100, 0));
				break;

			case P_GREEN:
				fieldStrip.setPixelColor(i - 1, fieldStrip.Color(0, 255, 0));
				break;

			case P_RED:
				fieldStrip.setPixelColor(i - 1, fieldStrip.Color(255, 0, 0));
				break;

			default:
				break;
		}
		fieldStrip.show();
	}
}

/**
 * @brief Clear a figure at given position (used for toggling)
 *
 * @param i Position of figure (field: 1...40, goal: -1...-4)
 * @param color Color of the figure, only needed when in goal,
 * 		use the defines P_BLUE, P_YELLOW, P_GREEN, P_RED
 */
void LED_CONTROLLER::clearFigure(int8_t i, u_int8_t color) {
	if (i <= 0) {
		switch (color) {
			case P_BLUE:
				goalStrip.setPixelColor(color * 4 - i - 1,
										goalStrip.Color(0, 0, 0));
				break;

			case P_YELLOW:
				goalStrip.setPixelColor(color * 4 + (4 + i),
										goalStrip.Color(0, 0, 0));
				break;

			case P_GREEN:
				goalStrip.setPixelColor(color * 4 + (4 + i),
										goalStrip.Color(0, 0, 0));
				break;

			case P_RED:
				goalStrip.setPixelColor(color * 4 + (4 + i),
										goalStrip.Color(0, 0, 0));
				break;

			default:
				break;
		}
		goalStrip.show();
	} else {
		fieldStrip.setPixelColor(i - 1, fieldStrip.Color(0, 0, 0));
		fieldStrip.show();
	}
}

void LED_CONTROLLER::clearAll() {
	baseStrip.clear();
	baseStrip.show();

	fieldStrip.clear();
	fieldStrip.show();

	goalStrip.clear();
	goalStrip.show();
}

// Deallocate LED object
LED_CONTROLLER::~LED_CONTROLLER() {
	fieldStrip.~Adafruit_NeoPixel();
	goalStrip.~Adafruit_NeoPixel();
	baseStrip.~Adafruit_NeoPixel();
}
