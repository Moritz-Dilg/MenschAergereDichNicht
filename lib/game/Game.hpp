#include <Arduino_GFX_Library.h>

class Game {
   public:
	Game(Arduino_GFX* gfx);
	~Game();

	/**
	 * @brief Called for every turn. Handles player
	 * selection, rolling dice, figure selection and movement.
	 */
	void turn();

	/**
	 * @brief Returns the figure at the given position
	 * if there is one.
	 *
	 * @param position The position to check for
	 * @return The figure at the given position or nullptr
	 */
	Figure* getFigureIfAtPosition(const short position);

   private:
	Player* players[4];
	LED_CONTROLLER* led;
	Arduino_GFX* gfx;
	short currentPlayer;
};