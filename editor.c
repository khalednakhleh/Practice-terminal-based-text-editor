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

}}

void EditorRefreshScreen(){

write(STDOUT_FILENO, "\x1b[2J", 4); // For hiding the cursor
/* 4 means we're typing 4 bytes to terminal. \x1b is esc "return 27", 
 followed by [. 2J clears the entire screen "1J clears from screen to cursor,
 and 0J from cursor to end of screen." */
write(STDOUT_FILENO, "\x1b[H", 3);
/* H is for repositing the cursor back to screen's top-left. */
EditorDrawRows();

write(STDOUT_FILENO, "\x1b[H", 3); // To reshow the cursor.
}

void EditorDrawRows(abuf *ab){
int y;
for(y = 0; y < E.screenrows; ++y){
		write(STDOUT_FILENO, "~", 1);

		if(y < E.screenrows - 1){
			write(STDOUT_FILENO, "\r\n", 2);
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







