#define __PLAYER__
#include <Arduino_GFX_Library.h>

#include "Figure.hpp"
#include "FigureSelector.hpp"
#include "LED.hpp"

#ifndef __GAME__
#include "Game.hpp"
#endif

class Player {
   public:
	/**
	 * @brief Construct a new Player object
	 *
	 * @param color The color of the player
	 * @param led The LED instance
	 */
	Player(short color, Game* game, LED* led, Arduino_GFX* gfx);
	~Player();

	/**
	 * @brief Called for every turn. Handles figure selection, rolling dice and
	 * movement.
	 */
	void turn();

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

   private:
	Figure* figures[4];
	LED* led;
	Game* game;
	Arduino_GFX* gfx;
	short figures_in_base;
	bool occupied_goal_positions[4];
	short selected_figure;
	short color;

	/**
	 * @brief Moves the figure by the given offset, handles defeating other
	 * figures and entering the goal
	 *
	 * @param figure The position of the figure to move
	 * @param offset The offset to move the figure by
	 * @return false if the figure is not in the game.
	 */
	bool move(short figure, short offset);

	/**
	 * @brief Simulates a dice roll. (MAYBE shows animation on display)
	 *
	 * @return The result of the dice roll
	 */
	u_int8_t roll_dice();
};