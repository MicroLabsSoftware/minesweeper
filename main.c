/* Minesweeper
   (C) Joonas Saarinen
   (C) MicroLabs
   This program is licensed under GNU General Public License (version 3),
   please see the included file gpl-3.0.txt for more details.
*/

#include "stdafx.h"
#include "minesweeper.h"

int main() {
	bool GameQuit = false;
	bool GameOver = false;
	bool Fresh = true;

	struct MinesweeperGameState GameState = {0};
	struct MinesweeperKeyboardState KeyboardState = {0};

	while (!GameQuit) {
		GameOver = false;
		Fresh = true;

		GameQuit = InitializeBasic(&GameState);

		while (!GameOver && !GameQuit) {

			RenderField(&GameState);

			ReadKeyboard(&GameState, &KeyboardState);
			
			if (KeyboardState.Valid == false) {
				PrintAndPause("Invalid command.");
			} else {
				if (KeyboardState.Command == Reveal) {

					if (Fresh) {
						InitializeExtended(&GameState, KeyboardState.x, KeyboardState.y);
						Fresh = false;
					}

					if (RevealTile(&GameState, KeyboardState.x, KeyboardState.y)) {
						RevealAllTiles(&GameState);
						RenderField(&GameState);
						PrintAndPause("BOOM! You're dead, pal!");
						GameOver = true;
					} else {
						if (Victory(&GameState)) {
							RevealAllTiles(&GameState);
							RenderField(&GameState);
							PrintAndPause("Victory!");
							GameOver = true;
						}
					}

				} else if (KeyboardState.Command == Flag) {

					if (!FlagTile(&GameState, KeyboardState.x, KeyboardState.y)) {
						PrintAndPause("Cannot flag that tile.");
					}

				} else if (KeyboardState.Command == Quit) {
					GameQuit = true;
				}
			}
		}

		printf("\n");

		FreeResources(&GameState);
	}

	return 0;
}