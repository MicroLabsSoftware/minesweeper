/* Minesweeper
   (C) Joonas Saarinen
   (C) MicroLabs
   This program is licensed under GNU General Public License (version 3),
   please see the included file gpl-3.0.txt for more details.
*/

#include "stdafx.h"
#include "minesweeper.h"

void PrintInformation() {
	printf("Minesweeper\n");
	printf("(C) Joonas Saarinen\n");
	printf("(C) MicroLabs\n\n");
	printf("This program is licensed under GNU General Public License (version 3).\n");
	printf("Please see the included file gpl-3.0.txt for more details.\n\n");

	return;
}

bool InitializeBasic(struct MinesweeperGameState *GameState) {
	/* Initialize a basic state where all tiles are hidden but otherwise
	   empty. Further initialization is done in InitializeExtended()
	   after the player has revealed the first tile. Returns true
	   if player wants to quit in the middle, otherwise false. */

	GameState->Valid = false;

	if (GetGameParameters(GameState) == true) return true;

	srand((unsigned int)time(NULL));

	GameState->Field = (unsigned char**)malloc(GameState->FieldY * sizeof(unsigned char *));

	if (GameState->Field == NULL) OutOfMemory();

	for (unsigned int r = 0; r < GameState->FieldY; r++) {
		GameState->Field[r] = (unsigned char*)malloc(GameState->FieldX * sizeof(unsigned char));
		if (GameState->Field[r] == NULL) OutOfMemory();
	}

	for (unsigned int y = 0; y < GameState->FieldY; y++) {
		for (unsigned int x = 0; x < GameState->FieldX; x++) {
			GameState->Field[y][x] = TILE_HIDDEN;
		}
	}

	GameState->Valid = true; // The game struct is properly initialized

	return false;
}

void InitializeExtended(struct MinesweeperGameState *GameState,
                        unsigned int FirstRevealX,
                        unsigned int FirstRevealY) {
	/* This function is called when the player reveals the very first tile.
	   This full initialization is not done earlier because we do not want
	   the first revealed tile to be a mine. */

	DistributeMines(GameState, GameState->MineCount, FirstRevealX, FirstRevealY);
	SetDigits(GameState);

	return;
}

void DistributeMines(struct MinesweeperGameState *GameState,
                     unsigned int Amount,
                     unsigned int DisallowedX, unsigned int DisallowedY) {

	/* AvailableMineLocations is a ramp from 0 to x*y that represents
	   each free tile by its 1-dimensional address.

	   Address = Y * FieldWidth + X
	   
	   When a tile location is consumed, it is replaced with the last tile
	   location in the list, and the size of the list
	   (AvailableMineLocationsSize) is decreased by 1. This method
	   works properly also when the consumed tile location is the last one.
	 */

	unsigned int AvailableMineLocationsSize = GameState->FieldY * GameState->FieldX;
	unsigned int *AvailableMineLocations = malloc(AvailableMineLocationsSize * sizeof(unsigned int));
	if (AvailableMineLocations == NULL) OutOfMemory();

	for (unsigned int m = 0; m < AvailableMineLocationsSize; m++) {
		AvailableMineLocations[m] = m;
	}

	/* Remove the first tile that the player revealed from the list of allowed
	   mine locations */

	AvailableMineLocations[DisallowedY * GameState->FieldX + DisallowedX] =
		AvailableMineLocations[AvailableMineLocationsSize - 1];

	AvailableMineLocationsSize--;

	for (unsigned int m = 0; m < Amount; m++) {

		// The field is full of mines (only occurs when an insane mine amount is defined)
		if (AvailableMineLocationsSize == 0) break;

		double RandomUnit = ((double)rand() / (double)RAND_MAX)
		                     * (double)(AvailableMineLocationsSize - 1);

		unsigned int PickMineIndex = (unsigned int)(round(RandomUnit));

		/* If rand() gets very close to RAND_MAX, floating point errors can push
		   the result of division over 1.0, so check that PickMineIndex is within
		   its upper bound */

		if (PickMineIndex > AvailableMineLocationsSize - 1)
			PickMineIndex = AvailableMineLocationsSize - 1;

		// Calculate the x/y address and plant the mine

		unsigned int MineY = AvailableMineLocations[PickMineIndex] / GameState->FieldX;
		unsigned int MineX = AvailableMineLocations[PickMineIndex] % GameState->FieldX;

		GameState->Field[MineY][MineX] |= TILE_MINE;

		AvailableMineLocations[PickMineIndex] =
			AvailableMineLocations[AvailableMineLocationsSize - 1];

		AvailableMineLocationsSize--;
	}

	free(AvailableMineLocations);

	return;
}

void SetDigits(struct MinesweeperGameState *GameState) {
	// Create mine count digits around mines

	for (unsigned int y = 0; y < GameState->FieldY; y++) {
		for (unsigned int x = 0; x < GameState->FieldX; x++) {

			// Skip if I am mine
			if (GameState->Field[y][x] & TILE_MINE) continue;

			// Check neighboring tiles for mines
			for (signed int dy = -1; dy <= 1; dy++) {
				for (signed int dx = -1; dx <= 1; dx++) {

					// Do not check myself
					if (dy == 0 && dx == 0) continue;

					// Calculate absolute coordinates of the tile to check
					signed int TestY = y + dy;
					signed int TestX = x + dx;

					/* Check if the neighboring tile would be outside of the
					   field (this happens when we scan the very borders of
					   the field), and skip tiles in such coordinates */

					if (TestY < 0 || TestY >= (signed int)GameState->FieldY) continue;
					if (TestX < 0 || TestX >= (signed int)GameState->FieldX) continue;

					unsigned int CheckMineY = (unsigned int)TestY;
					unsigned int CheckMineX = (unsigned int)TestX;

					if (GameState->Field[CheckMineY][CheckMineX] & TILE_MINE) {
						GameState->Field[y][x] |= TILE_DIGIT;
						GameState->Field[y][x]++;
					}
				}
			}
		}
	}

	return;
}

void FreeResources(struct MinesweeperGameState *GameState) {
	if (GameState->Valid) {
		for (unsigned int r = 0; r < GameState->FieldY; r++) {
			free(GameState->Field[r]);
		}

		free(GameState->Field);
	}

	return;
}

void OutOfMemory() {
	// Just exit the entire program if running out of memory
	printf("Out of memory!\n");
	exit(-1);
}