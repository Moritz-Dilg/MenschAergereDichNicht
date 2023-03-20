#include <Adafruit_NeoPixel.h>

#include "../types.hpp"

class LED_CONTROLLER {
   public:
	LED_CONTROLLER(u_int8_t brightness);
	~LED_CONTROLLER();

	void begin();
	void setBrightness(u_int8_t brightness);
	void setBase(u_int8_t n, u_int8_t color);
	void setGoal(u_int8_t n, u_int8_t color, int8_t from = -1);
	void setFigureToStart(u_int8_t color);
	void removeFigureFromField(u_int8_t position);
	void moveFigure(u_int8_t i, u_int8_t n, u_int8_t color);
	void setFigure(int8_t i, u_int8_t color);
	void clearFigure(int8_t i, u_int8_t color);
	void clearAll();

   private:
	bool verifyNFigures(u_int8_t n) { return n >= 0 && n <= 4; }
	void clearSection(u_int8_t index, Adafruit_NeoPixel &strip) {
		for (u_int8_t i = index * 4; i < index * 4 + 4; i++) {
			strip.setPixelColor(i, strip.Color(0, 0, 0));
		}
	}

   protected:
	u_int8_t brightness;
};
