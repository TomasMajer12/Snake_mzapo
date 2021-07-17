#ifndef MAIN_H
#define MAIN_H

//#define _POSIX_C_SOURCE 200112L
#define _GNU_SOURCE

#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include "utils.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>
#include <font_types.h>
#include <setjmp.h>
#include <pthread.h>

/*
  Structure DISPLAY with main informations about diplay
*/
typedef struct DISPLAY
{
  unsigned char *parlcd_mem_base;       //LDC screen mem_base
  unsigned short *fb;                   //Pointer to LCD screen
  unsigned char *led_stripe_mem_base;   //Led stripe mem_base
  unsigned char *RGB1_mem_base;         //RGB1 mem_base
  unsigned char *RGB2_mem_base;         //RGB2 mem_base
  int ptr;                              //Temporary ptr for screen refreshing
  struct termios oldt, newt;            //Utils for screen 
  bool first_menu;                      //Bool if it is first in menu
  bool menu;                            //Bool if it is in menu
  //bool led_start;                       //
  //uint32_t led_stripe_state;            //current state of Led stripe
  //uint32_t RGB1;                        //current state of RGB1
  //uint32_t RGB2;                        //current state of RGB2
} DISPLAY;

/*
  Struct GLOBAL_DATA for data, which need to be visible in every file of game
*/
typedef struct GLOBAL_DATA
{
  int refresh_time;     //refresh time = game speed
  int score;            //game score
  pthread_t *threads;   //array with created threads
  int color;            //setted snake color
  bool menu;            //BOOL if it is in menu
  bool is_on;           //BOOL that tells thread when to end
  bool food_led;        //BOOL for led blick after eating food
  bool death_led;
} GLOBAL_DATA;


GLOBAL_DATA global_data;  //Variable global data visible in every file od game


pthread_mutex_t mtx;      //Mutex key
pthread_cond_t condvar;   //Variable for thred broadcasting

/*
  Inicialize global data variable
*/
void global_data_init(void);

/*
  Inicializate everithing needed for game
  -Screen, RGB1, RGB2, global_data, LED stripe, threads variables
*/
void inicialization(void);

/*
  Main function for game
*/
int main(int argc, char *argv[]);
#endif
