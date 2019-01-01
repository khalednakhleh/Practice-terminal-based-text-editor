//  utils.c
//  kiloed
//
//  Created by Khaled Nakhleh on 12/30/18.
//  Copyright © 2018 Khaled Nakhleh. All rights reserved.
//  Implementation file for utils.h

#include "utils.h"
/* Defining structures */
EditorConfig E; 
abuf ab = ABUF_INIT;
DefWs ws;
/*---------------------*/

void EnableRawMode(){
    if(tcgetattr(STDIN_FILENO, &E.orig_termios) == -1) Die("Enable tcgetattr failed");
    atexit(DisableRawMode); //  This function is from stdlib.h. It is called when the main program ends, not the function.
    
    struct termios raw = E.orig_termios;
    raw.c_iflag &= ~(IXON | ICRNL | BRKINT | INPCK | ISTRIP);
    /*
     c_iflag is for input flags.
     IXON is an input flag for turning off CTRL-S and CTRL-Q.
     ICRNL is an input flag. turning it off to disable CTRL-M. CR stands for carriage return, NL for new line.
     BRKINT, if turned on, would send a break condition that causes a SIGINT, like CTRL-C.
     INPCK is for parity checking.
     ISTRIP makes the 8th bit of each byte be set to 0.
     */
    raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
    /*
     ECHO flag has the bit value: 00000000000000000000000000001000
     After flipping using ~ it becomes: 11111111111111111111111111110111
     c_lflag is a flag field that contains misc values.
     ISIG is for CTRL-C and CTRL-Z stopping and pausing the program, respectively.
     ICANON and ISIG are both misc flags. ICANON is a local flag in c_lflag. It turns off canonical mode.
     IEXTEN is a misc flag to disable CTRL-V*/
    
    raw.c_oflag &= ~(OPOST);
    /* c_oflag is for output flags.
     OPOST means post processing of output.*/
    
    raw.c_cflag |= (CS8);
    /* CS IS NOT A FLAG. It is a bit mask.  It sets the character size to 8 bits per byte.  | here means bitwise-OR.*/
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] =  1;
    if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) Die("Enable tcsetattr failed");
}

void DisableRawMode(){
    if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.orig_termios) == -1) Die("Disable tcgetattr failed"); // Returns the terminal back to original state.
}

void  Die(const char *s){
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);

    fprintf(stderr, "ERROR: %s: %s. Exiting...\n", s, strerror(errno));
    exit(1);
}

void AbAppend(abuf *b, const char *s, int len){
    char *new = realloc(b->b, b->len + len);

    if (new == NULL) return;
    memcpy(&new[b->len], s, len);
    b->b = new;
    b->len += len;
}

void AbFree(abuf *b){
    free(b->b);
}




