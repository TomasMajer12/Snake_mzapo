#ifndef SNAKE_H
#define SNAKE_H
#include "main.h"
#include <stdbool.h>

#define MULT_FACTOR 16  //real screen on MZ_APO is "multiplication_factor" times bigger than gaming grid
#define REFRESH_TIME 100 //refresh time of screen == game speed
#define GAME_WIDTH 29    //width of gaming grid
#define GAME_HEIGHT 15   //height of gaming grid
#define SNAKE_FOOD 1  //number of spawned food in grit

/*
  Struct of coordinations 
  -row represent yth element of grid
  -col represent xth element of grid
  -if more than one food spawned, index represent ith element in food array
*/

typedef struct Coords
{
  int row;
  int col;
  int index;
} Coords;


/*
  Struct with snake informations
  -direction = snake movement direction
  -leng = snake lenght
  -Coords head = coordinations of snakes head
  -Coords body[] = coordinations of whole snake, max snake lenght depends on gaming grid size
  -is_spawned = tells if snake is spawned
  -score = snake score
*/

typedef struct SNAKE
{
  char direction;
  int lenght;
  Coords head;
  Coords body[GAME_HEIGHT * GAME_WIDTH];
  bool is_spawned;
  int score;
} SNAKE;


/*
  Struct for gaming grid
  -0 -> free space in grid; 1 -> snake is in grid; 2 -> food is in grid;
  -g_width = gaming grid width
  -g_height = gaming grid height
  -multiplication_factor = real screen on MZ_APO is "multiplication_factor" times bigger than gaming grid
  -grid[GAME_HEIGHT * GAME_WIDTH] = array of gaming grid
  -game_is_on = tells if game is on
  -Coords food[SNAKE_FOOD] = array of coordinations with snake food
  -food_count = number of snake food
  -time = time of game
*/

typedef struct GAME_GRID
{
  int g_width;
  int g_height;
  int multiplication_factor; 
  int grid[GAME_HEIGHT * GAME_WIDTH];
  bool game_is_on;
  Coords food[SNAKE_FOOD];
  int food_count;
  int time;
} GAME_GRID;

/*
  Main snake game
  -gaming grid is inicialised and snake is spawned
  -game is controlled by user input from terminal
  -if corrent button is pressed, snake changes direction
  -it is checked, if snake died or ate the food
  -screen is refreshed 
*/
void main_game(DISPLAY *display);

/*
  Snake spawn
  -spawn snake on given coordinates
*/
void spawn_snake(Coords *start, SNAKE *snake);

/*
  Copy snake coordinates to gaming grid
*/
void copy_snake_to_grid(SNAKE *snake);

/*
  Copy food coordinates to gaming grid
*/
void copy_food_to_grid(void);

/*
  Creates food on random coordinates based on how many food is setted to spawn
*/
void create_random_food(DISPLAY *display);

/*
  Creates random food coordinates on i-th position in food array
*/
void create_random_food_index(int index);

/*
  Moves snake by one tile in gaming grid with defined direction "ch1"
*/
void move_snake(SNAKE *snake, char ch1, DISPLAY *display);

/*
  Checks if snake hitted the food, it he does, enlarge snake by one
*/
void check_food(SNAKE *snake, Coords *tmp_body, DISPLAY *display);

/*
  Set every tile in gaming grid to 0
*/
void clean_grid(void);

/*
  Check if snake hitted obstackle (it self)
*/
void check_obstacles(SNAKE *snake);

/*
  Inicializate grid
*/
bool grid_init(DISPLAY *display);

#endif