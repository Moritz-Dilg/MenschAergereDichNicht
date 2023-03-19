class Game;
class LED_CONTROLLER;
class Arduino_GFX;
class Figure;

class Player {
   public:
	bool occupied_goal_positions[4];

	/**
	 * @brief Construct a new Player object
	 *
	 * @param color The color of the player
	 * @param led The LED_CONTROLLER instance
	 */
	Player(short color, Game* game, LED_CONTROLLER* led, TFT_Display* tft);
	~Player();

	/**
	 * @brief Called for every turn. Handles figure selection, rolling dice and
	 * movement.
	 */
	bool turn();

	/**
	 * @brief Checks whether a given goal position is empty
	 *
	 * @param position The position to check for. Either 0, 1, 2 or 3
	 * @return true if the position is empty
	 */
	bool isGoalPosFree(short position);

	/**
	 * @brief Checks whether the player has at least one figure in the game
	 *
	 * @return The number of figures the player has in the game.
	 */
	short hasFiguresInGame();

	/**
	 * @brief Checks whether the player has at least one figure in the base
	 *
	 * @return The number of figures the player has in the base.
	 */
	short hasFiguresInBase();

	/**
	 * @brief Checks whether the player has all figures in the goal
	 *
	 * @return true if the player has all figures in the goal
	 */
	bool hasAllFiguresInGoal();

	/**
	 * @brief Removes the figure from the game if it is on the given position
	 *
	 * @param position The position to check for
	 * @return true if the figure was removed
	 */
	bool toBaseIfHit(short position);

	/**
	 * @brief Returns an array of the figure positions of this player
	 *
	 * @return An array of the figure positions of this player
	 */
	short* getPositions();

	/**
	 * @brief Returns the figure if it is at the given position or nullptr
	 * otherwise
	 *
	 * @param position The position to check for
	 * @return The figure if it is at the given position or nullptr
	 */
	Figure* getFigureIfAtPosition(const short position);

	/**
	 * @brief Checks whether there is an opposing figure at the specified
	 * position.
	 *
	 * @param position The position to check
	 * @return The figure if there is one
	 */
	Figure* getOpposingFigure(const short position);

	/**
	 * @brief Get number of Figures in Base
	 *
	 * @return number of Figures in Base
	 */
	short getFiguresInBase();

	/**
	 * @brief Returns a figure to base if it is at the given position
	 *
	 * @param position The position to check for
	 * @return true if the figure has been returned to base
	 * @return false if the figure has not been returned to base
	 */
	bool gameToBaseIfHit(const short position);

	/**
	 * @brief Returns the color of the player
	 *
	 * @return The color of the player
	 */
	short getColor();

   private:
	Figure* figures[4];
	LED_CONTROLLER* led;
	Game* game;
	TFT_Display* tft;
	short figures_in_base;
	Figure* selected_figure;
	short color;

	/**
	 * @brief Moves the figure by the given offset, handles defeating other
	 * figures and entering the goal
	 *
	 * @param figure The position of the figure to move
	 * @param offset The offset to move the figure by
	 * @return false if the figure is not in the game.
	 */
	bool move(Figure* figure, short offset);

	/**
	 * @brief Simulates a dice roll. (MAYBE shows animation on display)
	 *
	 * @return The result of the dice roll
	 */
	u_int8_t roll_dice();
};