class Player;
class LED_CONTROLLER;

class Figure {
   public:
	Figure(short color, Player* player, LED_CONTROLLER* led);

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
	 * @brief Returns the figure if it is at the given position or NULL
	 * otherwise
	 *
	 * @param position The position to check for
	 * @return The figure if it is at the given position or NULL
	 */
	Figure* getFigureIfAtPosition(short position);

	/**
	 * @brief Returns the position of the figure
	 *
	 * @return The position of the figure. -1 through -4 if the figure is in the
	 * goal. 0 if the figure is in the base.
	 */
	short getPosition();

	/**
	 * @return The number of figures currently in the base.
	 */
	short getInBase();

   private:
	short position;
	short color;
	Player* player;
	LED_CONTROLLER* led;
};