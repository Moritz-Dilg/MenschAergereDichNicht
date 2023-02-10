#include <Adafruit_NeoPixel.h>
#include <Arduino_GFX_Library.h>

#include "_game.hpp"

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

Game *game;

void setup(void) {
	// TFT
	gfx->begin();
	gfx->fillScreen(BLACK);

	/*gfx->setCursor(10, 10);
	gfx->setTextColor(WHITE);
	gfx->println("Hello World!");*/

	srand(time(NULL));
	Serial.begin(9600);
	Serial.println("main");
	game = new Game(gfx);
}

void loop() { game->turn(); }