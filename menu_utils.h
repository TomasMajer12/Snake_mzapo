#ifndef __MENU_UTILS_H__
#define __MENU_UTILS_H__


#include "font_types.h"
#include <string.h>
#include "main.h"

/*
    background color behind words
*/
void menu_line(int start, int end, DISPLAY *display);

/*
    clear background color behind words
*/
void clear_line_menu(DISPLAY *display);

/*
    returns width of given character 
*/
int Get_char_width(int character, font_descriptor_t *font_de);

/*
    draw given character on location x,y with given color and scale
*/
void draw_char(int x, int y, char ch, int scale, unsigned short color, unsigned short *fb, font_descriptor_t *fdes, DISPLAY *display);

/*
    splits words on characters and gives them to draw_char func
*/
void draw_word(int x, int y, char *word, int length, int scale, unsigned short color, unsigned short *fb, font_descriptor_t *fdes, int space, DISPLAY *display);

/*
    clear evevything on display
*/
void clear(DISPLAY *display);

/*
    draws rectangle on given coordinates 
*/
void draw_rectangle(int x, int y, int width, int height, unsigned short color, DISPLAY *display);

/*
    clear everything in line
*/
void clear_line(int start, int end, DISPLAY *display);

#endif
