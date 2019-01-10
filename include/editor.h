//  editor.h
//  kiloed
//
//  Created by Khaled Nakhleh on 01/03/19.
//  Copyright © 2019 Khaled Nakhleh. All rights reserved.
//  Header file for declarining editor's basic functions

#ifndef EDITOR_H_
#define EDITOR_H_

/*    ----- Macros and definitions -----   */

#define KILOED_VERSION "0.0.5"

/*    ----- structures' declarations -----   */

typedef enum{
	ARROW_UP = 1000,
	ARROW_DOWN,
	ARROW_LEFT,
	ARROW_RIGHT,
	PAGE_UP,
	PAGE_DOWN,
	HOME_KEY,
	END_KEY,
	ESC_KEY
}EditorKey;

/*    ----- Function prototypes -----   */

void InitEditor(); // Initialize editor parameters.
void EditorProcessKeyPress(); // Function that checks whether CTRL-q was placed to exit the program.
void EditorRefreshScreen(); // Refresing the screen after a single key press.
void EditorDrawRows(); // Drawing a single column of characters "usually tildes ~" for the given window size.
void EditorMoveCursor(int key); // Moving cursor using WASD keyboard keys.
int EditorReadKey(); /* Function that reads each keypress from the user,
	 also checks if an errnous value is placed,
	 for which the function exits the program with error value. */

/*    ----- Global variables -----   */

extern EditorKey ed;
extern abuf ab;

#endif /* EDITOR_H_ */






