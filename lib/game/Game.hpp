#include <sys/types.h>

#include "Player.hpp"

class Game {
   public:
	Game();
	~Game();

	/**
	 * @brief Called every tick to update the game state. Handles player
	 * selection, rolling dice, figure selection and movement.
	 *
	 * @return The color of the player that won the game or -1 if the game is
	 * still going.
	 */
	short tick();

   private:
	Player* players[4];
	LED led;
	short currentPlayer;

	/**
	 * @brief Moves the figure by the given offset, handles defeating other
	 * figures and entering the goal
	 *
	 * @param player The active player
	 * @param figure The position of the figure to move
	 * @param offset The offset to move the figure by
	 * @return false if the figure is not in the game.
	 */
	bool move(short player, short figure, short offset);

	/**
	 * @brief Selects the figure to continuously toggle the corresponding LED.
	 * Deselects any other figure.
	 *
	 * @param player The player to select the figure for
	 * @param figure The figure to select
	 */
	void select(short player, short figure);

	/**
	 * @brief Simulates a dice roll. (MAYBE shows animation on display)
	 *
	 * @return The result of the dice roll
	 */
	u_int8_t roll_dice();
};