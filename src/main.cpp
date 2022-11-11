#include <Adafruit_NeoPixel.h>
#include <Arduino_GFX_Library.h>

#define GFX_BL 22
#define GFX_CS 5
#define GFX_DC 19
#define GFX_RST 21
#define GFX_SCK 18
#define GFX_MOSI 23
#define GFX_MISO GFX_NOT_DEFINED
#define GFX_BGR false

Arduino_DataBus *bus =
	new Arduino_ESP32SPI(GFX_DC, GFX_CS, GFX_SCK, GFX_MOSI, GFX_MISO);

Arduino_GFX *gfx = new Arduino_ST7735(bus, GFX_RST, 0 /* rotation */, false,
									  128, 160, 0, 0, 0, 0, GFX_BGR);

Adafruit_NeoPixel strip = Adafruit_NeoPixel(5, 15, NEO_GRB + NEO_KHZ800);

uint32_t Wheel(byte WheelPos);

void setup(void) {
	// LED strip
	strip.begin();
	strip.setBrightness(50);
	strip.show();

	// Nextion TFT
	Serial2.begin(9600);
	Serial2.print("page page0");
	Serial2.print("\xFF\xFF\xFF");
	delay(50);
	Serial2.print("t0.txt=\"Hello World!\"");
	Serial2.print("\xFF\xFF\xFF");

	// TFT
	gfx->begin();
	gfx->fillScreen(BLACK);

	gfx->setCursor(10, 10);
	gfx->setTextColor(WHITE);
	gfx->println("Hello World!");

	gfx->setCursor(10, 30);
	gfx->setTextColor(RED);
	gfx->println("Hello World!");

	gfx->drawArc(50, 50, 50, 20, 0, 180, RED);
	gfx->drawCircle(50, 50, 45, GREEN);
	gfx->drawLine(50, 50, 100, 100, BLUE);
}

void loop() {
	// Rainbow fading animation of whole LED strip
	for (int j = 0; j < 256; j++) {
		for (int i = 0; i < strip.numPixels(); i++) {
			strip.setPixelColor(i, Wheel(((i + j) & 255)));
		}
		strip.show();
		delay(10);
	}
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
	if (WheelPos < 85) {
		return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
	} else if (WheelPos < 170) {
		WheelPos -= 85;
		return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
	}
	WheelPos -= 170;
	return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
}