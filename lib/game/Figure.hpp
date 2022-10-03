#include "../types.hpp"
#include "LED.hpp"

class Figure {
   public:
	Figure();

	/**
	 * @brief Moves the figure to the start position
	 *
	 * @return false if the figure is already in the game.
	 */
	bool toStart();

	/**
	 * @brief Moves the figure to the given position in the goal
	 *
	 * @param position The position in the goal. Either 1, 2, 3 or 4
	 * @return true if the figure was moved.
	 */
	bool toGoal(short position);

	/**
	 * @brief Moves the figure by the given offset, handles defeating other
	 * figures and entering the goal
	 *
	 * @param offset The offset to move the figure by
	 * @param led The LED instance
	 * @return false if the figure is not in the game.
	 */
	bool move(short offset, LED* led);

	/**
	 * @brief Removes the figure from the game if it is on the given position
	 *
	 * @param position The position to check for
	 * @param led The LED instance
	 * @return true if the figure was removed
	 */
	bool toBaseIfHit(short position, LED* led);

	/**
	 * @brief Selects the figure to continuously toggle the corresponding LED
	 *
	 * @param led The LED instance
	 * @return true if the figure was successfully selected
	 */
	bool select(LED* led);

	/**
	 * @brief Deselects the figure to stop continuously toggling the
	 * corresponding LED
	 *
	 * @param led The LED instance
	 * @return true if the figure was successfully deselected
	 */
	bool deselect(LED* led);

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
};