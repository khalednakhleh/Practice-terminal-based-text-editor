//  editor.c
//  kiloed
//
//  Created by Khaled Nakhleh on 12/31/18.
//  Copyright © 2018 Khaled Nakhleh. All rights reserved.
//  Implementation file for editor operations. Functions are declared in header file "utils.h"

#include "utils.h"

char EditorReadKey(){
int nread;
char c;

while ((nread = read(STDIN_FILENO, &c, 1) != 1)){
    if (nread == -1 && errno != EAGAIN) Die("Read function failed");
}
return c;
}

void EditorProcessKeyPress(){

char c = EditorReadKey();

switch (c) {
	case CTRL_KEY('q'):
	write(STDOUT_FILENO, "\x1b[2J", 4);
	write(STDOUT_FILENO, "\x1b[H", 3);
	exit (0);
	break;

	case 'w':
	case 's':
	case 'a':
	case 'd':
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
		char welcome[80];
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

int GetWindowSize(int *rows, int *cols){

if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
   if(write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12) return -1;  
   // C moves cursor right, B moves cursor down each by 999 value.s
   return GetCursorPosition(&E.screenrows, &E.screencols);}
else{
	*rows = ws.ws_row;  // Get number of terminal rows
	*cols = ws.ws_col; // Get number of terminal columns 
	return 0;
}}

void InitEditor(){
	E.cx = 0;
	E.cy = 0;
	if(GetWindowSize(&E.screenrows, &E.screencols) == -1) Die("InitEditor function failed");
}

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

void EditorMoveCursor(char key){
	switch(key){
		case 'a':
		E.cx--;
		break;
		case 'd':
		E.cx++;
		break;
		case 's':
		E.cy--;
		break;
		case 'w':
		E.cy++;
		break;
	}
}




