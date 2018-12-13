/* Minesweeper
   (C) Joonas Saarinen
   (C) MicroLabs
   This program is licensed under GNU General Public License (version 3),
   please see the included file gpl-3.0.txt for more details.
*/

#include "stdafx.h"
#include "minesweeper.h"

void ReadKeyboard(struct MinesweeperGameState *GameState,
                  struct MinesweeperKeyboardState *KeyboardState) {
	char KeyboardBuffer[8] = {0};

	KeyboardState->Valid = false;

	char HighestLetter = 'A' + (GameState->FieldX - 1);
	
	printf(": ");

	fgets(KeyboardBuffer, 8, stdin);

	// Replace line feed with null
	KeyboardBuffer[strcspn(KeyboardBuffer, "\n")] = 0;

	// Convert to uppercase
	for (unsigned char c = 0; c < strlen(KeyboardBuffer); c++) {
		KeyboardBuffer[c] = toupper(KeyboardBuffer[c]);
	}

	if (strlen(KeyboardBuffer) == 1) {
		// Quit command
		if (KeyboardBuffer[0] == 'Q') {
			KeyboardState->Valid = true;
			KeyboardState->Command = Quit;
		}
	} else if (strlen(KeyboardBuffer) == 2) {
		// Reveal command

		/* LetterFirst is true when the command is valid and X axis (referenced
		   by a letter) comes first. NumberFirst is true when the command is
		   valid and Y axis (referenced by a number) comes first. Both
		   LetterFirst and NumberFirst are false when the command is invalid. */

		bool LetterFirst = true;
		bool NumberFirst = true;

		LetterFirst &= (KeyboardBuffer[0] >= 'A');
		LetterFirst &= (KeyboardBuffer[0] <= HighestLetter);
		LetterFirst &= (KeyboardBuffer[1] >= '0');
		LetterFirst &= (KeyboardBuffer[1] <= '9');

		NumberFirst &= (KeyboardBuffer[0] >= '0');
		NumberFirst &= (KeyboardBuffer[0] <= '9');
		NumberFirst &= (KeyboardBuffer[1] >= 'A');
		NumberFirst &= (KeyboardBuffer[1] <= HighestLetter);

		if (LetterFirst) {
			KeyboardState->Valid = true;
			KeyboardState->Command = Reveal;
			KeyboardState->x = KeyboardBuffer[0] - 'A';
			KeyboardState->y = KeyboardBuffer[1] - '0';
		} else if (NumberFirst) {
			KeyboardState->Valid = true;
			KeyboardState->Command = Reveal;
			KeyboardState->x = KeyboardBuffer[1] - 'A';
			KeyboardState->y = KeyboardBuffer[0] - '0';
		}
	} else if (strlen(KeyboardBuffer) == 3) {
		// Flag command
		if (KeyboardBuffer[0] == 'F') {
			/* LetterFirst is true when the command is valid and X axis (referenced
			   by a letter) comes first. NumberFirst is true when the command is
			   valid and Y axis (referenced by a number) comes first. Both
			   LetterFirst and NumberFirst are false when the command is invalid. */

			bool LetterFirst = true;
			bool NumberFirst = true;

			LetterFirst &= (KeyboardBuffer[1] >= 'A');
			LetterFirst &= (KeyboardBuffer[1] <= HighestLetter);
			LetterFirst &= (KeyboardBuffer[2] >= '0');
			LetterFirst &= (KeyboardBuffer[2] <= '9');

			NumberFirst &= (KeyboardBuffer[1] >= '0');
			NumberFirst &= (KeyboardBuffer[1] <= '9');
			NumberFirst &= (KeyboardBuffer[2] >= 'A');
			NumberFirst &= (KeyboardBuffer[2] <= HighestLetter);

			if (LetterFirst) {
				KeyboardState->Valid = true;
				KeyboardState->Command = Flag;
				KeyboardState->x = KeyboardBuffer[1] - 'A';
				KeyboardState->y = KeyboardBuffer[2] - '0';
			} else if (NumberFirst) {
				KeyboardState->Valid = true;
				KeyboardState->Command = Flag;
				KeyboardState->x = KeyboardBuffer[2] - 'A';
				KeyboardState->y = KeyboardBuffer[1] - '0';
			}
		}
	}

	return;
}

char GetSingleCharacterChoice(char *Choices) {
	/* Asks player to choose one letter from a menu. The Choices is
	   a string of all acceptable characters, and returned is the character
	   of the chosen letter. */

	char KeyboardBuffer[8] = {0};

	bool Acceptable = false;
	unsigned int AcceptedChoice = 0;

	while (!Acceptable) {
		/* Keep bringing up the prompt until the player enters a character
		   that is one of the acceptable choices */

		printf(": ");

		fgets(KeyboardBuffer, 8, stdin);

		// Replace line feed with null
		KeyboardBuffer[strcspn(KeyboardBuffer, "\n")] = 0;

		for (unsigned int c = 0; c < strlen(Choices); c++) {
			if (KeyboardBuffer[0] == Choices[c]) {
				Acceptable = true;
				AcceptedChoice = Choices[c];
				break; // If there is a match, will not check other characters
			}
		}
	}

	return AcceptedChoice;
}

bool GetGameParameters(struct MinesweeperGameState *GameState) {
	/* Ask desired board size and mine amount from player.
	   Returns true if player wants to quit, false otherwise. */

	printf("Choose size of board\n");
	printf("a) 26 x 10\n");
	printf("b) 13 x 10\n");
	printf("q) Quit\n");

	switch(GetSingleCharacterChoice("abq")) {
		case 'a':
			GameState->FieldX = 26;
			GameState->FieldY = 10;
			break;
		case 'b':
			GameState->FieldX = 13;
			GameState->FieldY = 10;
			break;
		case 'q':
			return true;
	}

	printf("\n");
	printf("Choose amount of mines\n");
	printf("a) 5\n");
	printf("b) 10\n");
	printf("c) 20\n");
	printf("q) Quit\n");

	switch(GetSingleCharacterChoice("abcq")) {
		case 'a':
			GameState->MineCount = 5;
			break;
		case 'b':
			GameState->MineCount = 10;
			break;
		case 'c':
			GameState->MineCount = 20;
			break;
		case 'q':
			return true;
	}

	return false;
}

void PrintAndPause(char *Message) {
	printf("%s\n\nPress Enter to continue...", Message);

	char c = 0;
	while ((c = getchar()) != '\n' && c != EOF);

	return;
}