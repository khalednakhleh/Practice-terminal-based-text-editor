//  main.c
//  kiloed
//
//  Created by Khaled Nakhleh on 12/30/18.
//  Copyright © 2018 Khaled Nakhleh. All rights reserved.

#include "utils.h"

// Declaring global variables
    EditorConfig E;
    DefWs ws;
    EditorKey ed;

int main(void){

    EnableRawMode();
    InitEditor();
    
    while(1){
        EditorRefreshScreen();
        EditorProcessKeyPress();
    }
    
    
    
    return EXIT_SUCCESS;
}
