/* Minesweeper
   (C) Joonas Saarinen
   (C) MicroLabs
   This program is licensed under GNU General Public License (version 3),
   please see the included file gpl-3.0.txt for more details.
*/

#include "stdafx.h"
#include "minesweeper.h"

void RenderField(struct MinesweeperGameState *GameState) {
	// Too large field to render nicely by this function
	if (GameState->FieldX > 26 || GameState->FieldY > 10) return;

	/* There is no portable way to clear the screen in C, so the screen is
	   cleared using ANSI escape sequence. This requires Windows 10, because
	   Command Prompt in older versions of Windows does not support ANSI
	   commands. */

	printf("\033[2J");

	// One extra zero as null termination
	char *LineBuffer = calloc(GameState->FieldX + 1, sizeof(char));
	
	// Horizontal axis reference

	printf("  ");

	for (unsigned int r = 0; r < GameState->FieldX; r++) {
		LineBuffer[r] = 'A' + r;
	}

	printf("%s\n", LineBuffer);

	memset(LineBuffer, 0x00, GameState->FieldX + 1);

	for (unsigned int y = 0; y < GameState->FieldY; y++) {
		for (unsigned int x = 0; x < GameState->FieldX; x++) {

			// Choose which character to draw based on the bits set in the tile

			char RenderCharacter = 0;

			// Tiles have a priority by which they are shown
			if (GameState->Field[y][x] & TILE_FLAG) {
				RenderCharacter = 'F';
			} else if (GameState->Field[y][x] & TILE_HIDDEN) {
				RenderCharacter = '#';
			} else if (GameState->Field[y][x] & TILE_MINE) {
				RenderCharacter = '*';
			} else if (GameState->Field[y][x] & TILE_DIGIT) {
				RenderCharacter = '0' + (GameState->Field[y][x] & TILE_DIGIT_VALUE);
			} else {
				RenderCharacter = ' ';
			}

			LineBuffer[x] = RenderCharacter;
		}

		// y + '0' is vertical axis reference
		printf("%c %s\n", y + '0', LineBuffer);
	}

	free(LineBuffer);

	printf("\n");
	printf("xy    Reveal a tile\n");
	printf("Fxy   Flag a tile\n");
	printf("Q     Quit game\n");
	printf("\n");

	return;
}