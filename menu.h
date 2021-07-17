#ifndef MENU_H
#define MENU_H

#include "font_types.h"
#include <string.h>

/*
    render the speed adjustment menu
*/
void speed_opt_menu(DISPLAY *display, font_descriptor_t *font_des);


/*
    render the main menu
*/
void menu(font_descriptor_t *font_des, DISPLAY *display);


/*
    render game settings
*/
void options_menu(DISPLAY *display, font_descriptor_t *font_des);


/*
    render End Screen menu
    jmp_buf jmp_menu is jump back to main menu
    
*/
void end_screen(DISPLAY *display, font_descriptor_t *font_des, jmp_buf game, jmp_buf jmp_menu, int score);


/*
    render the color adjustment menu
*/
void color_menu(DISPLAY *display, font_descriptor_t *font_des);

#endif
