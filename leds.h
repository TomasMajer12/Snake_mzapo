#ifndef __LEDS_H__
#define __LEDS_H__

#include "main.h"

#define RED_LED 0x00FF0000
#define GREEN_LED 0x0000FF00
#define BLUE_LED 0x000000FF
#define OFF_LED 0x00000000
/*
    Thread for led diodes
    -thread for led blinking while in menu
    -blinks RGB diodes red if snake eats food
*/
void *led_thread(void *displ);

/*
    Inicializate led stripe on MZ-APO
*/
void led_base_init(DISPLAY *display);

/*
    Inicializate RGB1 on MZ-APO
*/
void RGB1_init(DISPLAY *display);

/*
    Inicializate RGB2 on MZ-APO
*/
void RGB2_init(DISPLAY *display);

/*
    Light whole led stripe
*/
void full_led_line(unsigned char *membase);

/*
    Turn off whole led stripe
*/
void clear_led_line(unsigned char *membase);

/*
    Makes led animation
    -led stripe lights up from left side to right and than turns off backwards 
    -time of animation depends on delay
*/
void moving_led(unsigned char *membase, int delay);

/*
    Displays current snake score on led stripe
    -if score is bigger than led stripe size, led stripe is reseted
*/
void led_food_up(DISPLAY *display, bool reset);

/*
    Changes RGB1 color to given color
*/
void RGB1_color(DISPLAY *display, int color);

/*
    Changes RGB2 color to given color
*/
void RGB2_color(DISPLAY *display, int color);

/*
    Changes both RGB1 and RGB2 color to given collor
*/
void both_RGB_color(DISPLAY *display, int color);

/*
    Led stripe animation while in Menu
*/
void menu_led_animation(DISPLAY *display);
#endif