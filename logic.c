/* Minesweeper
   (C) Joonas Saarinen
   (C) MicroLabs
   This program is licensed under GNU General Public License (version 3),
   please see the included file gpl-3.0.txt for more details.
*/

#include "stdafx.h"
#include "minesweeper.h"

bool RevealTile(struct MinesweeperGameState *GameState, unsigned int x, unsigned int y) {
	/* Reveals a tile chosen by the player. Returns true if the tile was a mine,
	   otherwise false. */

	// Tile already revealed
	if (!(GameState->Field[y][x] & TILE_HIDDEN)) {
		PrintAndPause("Tile has already been revealed.");
		return false;
	}

	// Safeguard to prevent player from revealing tiles that he has flagged
	if (GameState->Field[y][x] & TILE_FLAG) {
		PrintAndPause("Cannot reveal a flagged tile.");
		return false;
	}

	// Mine exploded
	if (GameState->Field[y][x] & TILE_MINE) {
		return true;
	}

	// Mine did not explode. Now reveal the tile and adjacent empty tiles.
	RecursiveFloodFill(GameState, x, y);

	return false;
}

void RevealAllTiles(struct MinesweeperGameState *GameState) {
	// Reveal all tiles when game is won or lost

	for (unsigned int y = 0; y < GameState->FieldY; y++) {
		for (unsigned int x = 0; x < GameState->FieldX; x++) {
			GameState->Field[y][x] &= ~TILE_HIDDEN;
		}
	}

	return;
}

bool FlagTile(struct MinesweeperGameState *GameState, unsigned int x, unsigned int y) {
	/* Flags or unflags a tile chosen by player. Returns true if command was
	   successful, otherwise false. */

	if (GameState->Field[y][x] & TILE_FLAG) {
		// Flag present, remove it
		GameState->Field[y][x] &= ~TILE_FLAG;
		return true;
	}

	if (!(GameState->Field[y][x] & TILE_HIDDEN)) {
		// Tile revealed, cannot flag such tile
		return false;
	}

	// Apply flag
	GameState->Field[y][x] |= TILE_FLAG;

	return true;
}

void RecursiveFloodFill(struct MinesweeperGameState *GameState,
                        unsigned int x, unsigned int y) {
	/* Reveal tiles recursively until a digit tile is reached. This is
	   called when the player has revealed a tile and it has been checked
	   that it is not a mine and that the tile has not already been revealed. */

	// Reveal tile
	GameState->Field[y][x] &= ~TILE_HIDDEN;

	// Do not expand further if digit
	if (GameState->Field[y][x] & TILE_DIGIT) return;

	for (signed int cy = -1; cy <= 1; cy++) {
		for (signed int cx = -1; cx <= 1; cx++) {

			// The tile itself or diagonal tiles are not expanded into
			if (cx == cy) continue;

			signed int TestY = cy + (signed int)y;
			signed int TestX = cx + (signed int)x;

			// Do not expand outside field
			if (TestY < 0 || TestY >= (signed int)GameState->FieldY) continue;
			if (TestX < 0 || TestX >= (signed int)GameState->FieldX) continue;

			// Check if the expand candidate is already revealed
			if (!(GameState->Field[TestY][TestX] & TILE_HIDDEN)) continue;

			// Can expand
			RecursiveFloodFill(GameState, TestX, TestY);
		}
	}

	return;
}

bool Victory(struct MinesweeperGameState *GameState) {
	/* Returns true if the game has been won, otherwise false. Victory is
	   defined as a condition where all tiles but mine tiles have been
	   revealed. */

	bool Victory = true;

	for (unsigned int y = 0; y < GameState->FieldY; y++) {
		for (unsigned int x = 0; x < GameState->FieldX; x++) {
			// If tile is hidden and not a mine
			if ((GameState->Field[y][x] & TILE_HIDDEN) &&
				!(GameState->Field[y][x] & TILE_MINE)) {
				Victory = false;
			}
		}
	}

	return Victory;
}