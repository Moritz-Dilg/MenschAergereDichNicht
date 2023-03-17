
class Game {
   public:
	Game(TFT_Display* tft, LED_CONTROLLER* led, short player_count = 4);
	~Game();

	/**
	 * @brief Called for every turn. Handles player
	 * selection, rolling dice, figure selection and movement.
	 *
	 * @return true The game is over
	 * @return false The game is still running
	 */
	bool turn();

	/**
	 * @brief Returns the figure at the given position
	 * if there is one.
	 *
	 * @param position The position to check for
	 * @return The figure at the given position or nullptr
	 */
	Figure* getFigureIfAtPosition(const short position);

	/**
	 * @brief Puts any figure in base, if at position
	 *
	 * @param position The position to check for
	 * @return true A Figure has been returned to base
	 * @return false No Figure has been retruned to base
	 */
	bool toBaseIfHit(const short position);

   private:
	Player* players[4];
	LED_CONTROLLER* led;
	TFT_Display* tft;
	short currentPlayer;
	short player_count;
};