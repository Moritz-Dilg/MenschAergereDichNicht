#define __GAME__
#include <sys/types.h>

#include "Player.hpp"

class Game {
   public:
	Game();
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
	LED* led;
	short currentPlayer;
};