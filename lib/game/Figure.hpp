#include "../types.hpp"
#include "LED.hpp"

class Figure {
   public:
	Figure(short color, LED* led);

	/**
	 * @brief Moves the figure to the start position
	 *
	 * @param players_in_base Number of players in base
	 * @return false if the figure is already in the game.
	 */
	bool toStart(short& players_in_base);

	/**
	 * @brief Moves the figure by the given offset, handles defeating other
	 * figures and entering the goal
	 *
	 * @param offset The offset to move the figure by
	 * @return false if the figure is not in the game.
	 */
	bool move(short offset);

	/**
	 * @brief Removes the figure from the game if it is on the given position
	 *
	 * @param position The position to check for
	 * @param players_in_base Number of players in base
	 * @return true if the figure was removed
	 */
	bool toBaseIfHit(short position, short& players_in_base);

	/**
	 * @brief Selects the figure to continuously toggle the corresponding LED
	 */
	void select();

	/**
	 * @brief Deselects the figure to stop continuously toggling the
	 * corresponding LED
	 */
	void deselect();

	/**
	 * @brief Returns the figure if it is at the given position or NULL
	 * otherwise
	 *
	 * @param position The position to check for
	 * @return The figure if it is at the given position or NULL
	 */
	Figure* getFigureIfAtPosition(short position);

   private:
	short position;
	short color;
	LED* led;
};