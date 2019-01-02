//  editor.c
//  kiloed
//
//  Created by Khaled Nakhleh on 12/31/18.
//  Copyright © 2018 Khaled Nakhleh. All rights reserved.
//  Implementation file for editor operations. Functions are declared in header file "utils.h"

#include "utils.h"

void InitEditor(){
	E.cx = 0;
	E.cy = 0;
	if(GetWindowSize(&E.screenrows, &E.screencols) == -1) Die("InitEditor function failed");
}

int GetWindowSize(int *rows, int *cols){

if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
   if(write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12) return -1;  
   // C moves cursor right, B moves cursor down each by 999 values
   return GetCursorPosition(&E.screenrows, &E.screencols);}
else{
	*rows = ws.ws_row;  // Get number of terminal rows
	*cols = ws.ws_col; // Get number of terminal columns 
	return 0;
}}

int GetCursorPosition(int *rows, int *cols){
	char buf[32];
	unsigned int i = 0;

	if(write(STDOUT_FILENO, "\x1b[6n", 4) != 4) return -1;

	while (i < (sizeof(buf) - 1)){
	if(read(STDIN_FILENO, &buf[i],  1) != 1) break;
	if(buf[i] == 'R') break;
	++i;
	}
	buf[i] = '\0';
	if(buf[0] != '\x1b' || buf[1] != '[') return -1;
	if(sscanf(&buf[2], "%d;%d", rows, cols) != 2) return -1; 

	return 0;
}

int EditorReadKey(){
int nread;
char c;

while ((nread = read(STDIN_FILENO, &c, 1) != 1)){
    if (nread == -1 && errno != EAGAIN) Die("Read function failed");
}
if (c == '\x1b'){
	char seq[3];

	if(read(STDOUT_FILENO, &seq[0], 1) != 1) return '\x1b';
	if(read(STDOUT_FILENO, &seq[1], 1) != 1) return '\x1b';
	// Aliasing the arrow keys, and mapping them to WASD for cursor movement
if(seq[0] == '['){
	switch(seq[1]){
		case 'A': return ARROW_UP;
		case 'B': return ARROW_DOWN;
		case 'C': return ARROW_RIGHT;
		case 'D': return ARROW_LEFT;}
	}
	 return '\x1b';}
	 else{
return c;
}}

void EditorProcessKeyPress(){
int c = EditorReadKey();

switch (c) {
	case CTRL_KEY('q'):  // For exiting the editor program
	write(STDOUT_FILENO, "\x1b[2J", 4);
	write(STDOUT_FILENO, "\x1b[H", 3);
	exit (0);
	break;

	case ARROW_UP:
	case ARROW_DOWN:
	case ARROW_LEFT:
	case ARROW_RIGHT:
		EditorMoveCursor(c);
		break;

}}

void EditorRefreshScreen(){

abuf ab = ABUF_INIT;

AbAppend(&ab, "\x1b[?25l", 6); // For hiding the cursor
/* 4 means we're typing 4 bytes to terminal. \x1b is esc "return 27", 
 followed by [. 2J clears the entire screen "1J clears from screen to cursor,
 and 0J from cursor to end of screen." */
AbAppend(&ab, "\x1b[H", 3);
/* H is for repositing the cursor back to screen's top-left. */
EditorDrawRows(&ab);

char buf[32];
snprintf(buf, sizeof(buf), "\x1b[%d;%dH", E.cy + 1, E.cx + 1);
AbAppend(&ab, buf, strlen(buf));

AbAppend(&ab, "\x1b[?25h", 6);

write(STDOUT_FILENO, ab.b, ab.len);
AbFree(&ab);
}

void EditorDrawRows(abuf *ab){
int y;
for(y = 0; y < E.screenrows; ++y){
	if(y == E.screenrows / 3){
		char welcome[50];
		int welcomelen = snprintf(welcome, sizeof(welcome), "Kiloed Editor -- Version %s", KILOED_VERSION);
		if (welcomelen > E.screencols) welcomelen = E.screencols;
		int padding = (E.screencols - welcomelen) / 2;
		if (padding){
			AbAppend(ab, "~", 1);
			--padding;
		}
		while(--padding) AbAppend(ab, " ", 1);
		AbAppend(ab, welcome, welcomelen);
	}
		AbAppend(ab, "~", 1);
		AbAppend(ab, "\x1b[K", 3);
		if(y < E.screenrows - 1){
			AbAppend(ab, "\r\n", 2);
		}

	}}

void EditorMoveCursor(int key){
	switch(key){
		case ARROW_LEFT:
		if(E.cx != 0){
		E.cx--;
		}
		break;
		case ARROW_RIGHT:
		if (E.cx != E.screencols - 1){
		E.cx++;
		}
		break;
		case ARROW_DOWN:
		if (E.cy != E.screenrows - 1){
		E.cy++;
		}
		break;
		case ARROW_UP:
		if (E.cy != 0){
		E.cy--;
		}
		break;
	}}

