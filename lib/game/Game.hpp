
class Game {
   public:
	Game(TFT_Display* tft);
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
};