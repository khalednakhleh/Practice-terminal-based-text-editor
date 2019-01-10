//  utils.h
//  kiloed
//
//  Created by Khaled Nakhleh on 12/30/18.
//  Copyright © 2018 Khaled Nakhleh. All rights reserved.
//  Header file that declares function prototypes, macros, and structures. 

#ifndef UTILS_H_
#define UTILS_H_

/*    ----- Include header files ----- */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>    // For read(), STDIN_FILENO, STDOUT_FILENO, write()
#include <termios.h>   // For struct termios, tcsetattr(), tcgetattr(), ECHO, TCSAFLUSH
#include <ctype.h>
#include <errno.h>
#include <string.h>    // for strerror(), memcpy()
#include <sys/ioctl.h> // for TIOCGWINSZ meaning Terminal Input Output Control Get WINdow SiZe.

/*    ----- Macros and definitions -----   */

#define CTRL_KEY(k) ((k) & 0x1f)
#define ABUF_INIT {NULL, 0}

/*    ----- structures' declarations -----   */

typedef struct{
    char *b;
    int len;
}abuf;

typedef struct{
	int cx, cy;
	int screenrows;
	int screencols;
	struct termios orig_termios;
}EditorConfig;

typedef struct winsize DefWs;

/*    ----- Function prototypes -----   */

void EnableRawMode(void); /* Function EnableRawMode() converts the terminal from canonical (cooked) mode to raw mode. 
 Function EnableRawMode() doesnt show the characters being typed in the terminal.Function implementation can be found in attributes.c */
void DisableRawMode(void); // Function DisableRawMode() reverts the terminal back to cooked mode.
void Die(const char *s); // Error handling function. Prints out an error string if a function fails.
int GetWindowSize(int *rows, int *columns);
int GetCursorPosition(int *rows, int *cols); // Get position using ioctl(). In case of failure use a custom-built function.
void AbAppend(abuf *b, const char *s, int len);
void AbFree(abuf *b); // Freeing string member of append buffer.

/*    ----- Global variables -----   */

extern EditorConfig E;
extern DefWs ws;


#endif /* UTILS_H_ */


