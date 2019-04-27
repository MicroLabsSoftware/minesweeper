/* Minesweeper
   (C) Joonas Saarinen
   (C) MicroLabs
   This program is licensed under GNU General Public License (version 3),
   please see the included file gpl-3.0.txt for more details.
*/

#pragma once

struct MinesweeperGameState {
	bool Valid;
	unsigned int FieldY;
	unsigned int FieldX;
	unsigned char **Field;
	unsigned int MineCount;
};

struct MinesweeperKeyboardState {
	bool Valid;
	enum {Reveal, Flag, Quit} CommandEnum;
	enum CommandEnum Command;
	unsigned int x;
	unsigned int y;
};

// Bitmasks for tile properties
#define TILE_HIDDEN      0x80 // bit 7 (MSB)
#define TILE_FLAG        0x40 // bit 6
#define TILE_MINE        0x20 // bit 5
#define TILE_DIGIT       0x10 // bit 4
#define TILE_DIGIT_VALUE 0xF  // bit 3..0

bool RevealTile(struct MinesweeperGameState *GameState, unsigned int x, unsigned int y);
void RevealAllTiles(struct MinesweeperGameState *GameState);
bool FlagTile(struct MinesweeperGameState *GameState, unsigned int x, unsigned int y);
void RecursiveFloodFill(struct MinesweeperGameState *GameState, unsigned int x, unsigned int y);
bool Victory(struct MinesweeperGameState *GameState);
void RenderField(struct MinesweeperGameState *GameState);
void PrintAndPause(char *Message);
void PrintInformation();
bool InitializeBasic(struct MinesweeperGameState *GameState);
void InitializeExtended(struct MinesweeperGameState *GameState, unsigned int FirstRevealX, unsigned int FirstRevealY);
void DistributeMines(struct MinesweeperGameState *GameState, unsigned int Amount, unsigned int DisallowedX, unsigned int DisallowedY);
void SetDigits(struct MinesweeperGameState *GameState);
void ReadKeyboard(struct MinesweeperGameState *GameState, struct MinesweeperKeyboardState *KeyboardState);
char GetSingleCharacterChoice(char *Choices);
bool GetGameParameters(struct MinesweeperGameState *GameState);
void FreeResources(struct MinesweeperGameState *GameState);
void OutOfMemory();