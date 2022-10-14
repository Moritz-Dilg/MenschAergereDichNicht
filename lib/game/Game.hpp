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

   private:
	Player* players[4];
	LED* led;
	short currentPlayer;
};