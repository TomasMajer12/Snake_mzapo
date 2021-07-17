#ifndef DISP_INIT_H
#define DISP_INIT_H

#include "snake.h"

/*
    colors
*/
#define WHITE_COLOR 0xFFFF
#define RED_COLOR 0xF800
#define BLUE_COLLOR 0x001F
#define LIGHT_GREY 0xBDF7
#define DARK_GREY 0x7BEF
#define GREEN_COLOR 0x07E0
#define YELLOW_COLOR 0xFF00

/*
    display initializacion
*/
void display_init(DISPLAY *display);

/*
    colors coordinates x,y with given color
*/
void draw_pixel(int x, int y, unsigned short color, DISPLAY *display);

/*
    colors coordinates x,y with given color in given scale
*/
void draw_pixel_scale(int x, int y, int scale, unsigned short color, DISPLAY *display);

/*
    refresh display
*/
void refresh_screen(DISPLAY *display);

/*
    draws snake grid in display
*/
void draw_grid_in_screen(GAME_GRID *field, DISPLAY *display);

/*
    visual site of game, draws current score and time on display
*/
void game_mask(DISPLAY *display, GAME_GRID *field, SNAKE *snake);

#endif
