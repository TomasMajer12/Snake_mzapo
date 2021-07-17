/*******************************************************************
 * file with snake game
 *******************************************************************/

#include "snake.h"
#include "display.h"
#include "leds.h"
#include "menu_utils.h"

#include <time.h>

GAME_GRID field;

void main_game(DISPLAY *display)
{

  //GRID inicialization
  pthread_mutex_lock(&mtx);
  global_data.menu = false;
  pthread_mutex_unlock(&mtx);

  clear(display);
  grid_init(display);

  time_t t1 = time(NULL);

  //Snake inicialization
  SNAKE snake_1;
  Coords start = {.col = 2, .row = 0,  .index = 0};
  spawn_snake(&start, &snake_1);

  //Game interface
  game_mask(display, &field, &snake_1);
  //blinking_led_line(display->led_stripe_mem_base, 2);

  moving_led(display->led_stripe_mem_base, 10);
  create_random_food(display);
  //clear_led_line(display->led_stripe_mem_base);

  for (display->ptr = 0; display->ptr < 320 * 480; display->ptr++)
  {
    display->fb[display->ptr] = 0u;
  }
  draw_grid_in_screen(&field, display);
  refresh_screen(display);

  struct timespec loop_delay;
  loop_delay.tv_sec = 0;
  loop_delay.tv_nsec = global_data.refresh_time * 1000 * 1000;

  clock_nanosleep(CLOCK_MONOTONIC, 0, &loop_delay, NULL);

  char ch1 = ' ';

  while (field.game_is_on)
   {
    for (display->ptr = 0; display->ptr < 320 * 480; display->ptr++)
    {
      display->fb[display->ptr] = 0u;
    }

    int r = read(0, &ch1, 1);
    if (r == 0)
    {
      move_snake(&snake_1, snake_1.direction, display);
    }

    while (r == 1)
    {
      if (ch1 == 'w' || ch1 == 'a' || ch1 == 's' || ch1 == 'd')
      {
        move_snake(&snake_1, ch1, display);
      }
      else if (ch1 == 'q')
      {
        global_data.score = snake_1.score;
        field.game_is_on = false;
      }
      r = read(0, &ch1, 1);
    }
    field.time = (int)difftime(time(NULL), t1);
    clean_grid();

    copy_food_to_grid();

    copy_snake_to_grid(&snake_1);

    draw_grid_in_screen(&field, display);
    game_mask(display, &field, &snake_1);
    refresh_screen(display);

    clock_nanosleep(CLOCK_MONOTONIC, 0, &loop_delay, NULL);
  }

  led_food_up(display, true);
  clear_led_line(display->led_stripe_mem_base);
  pthread_mutex_lock(&mtx);
  global_data.death_led = true;
  pthread_cond_broadcast(&condvar);
  pthread_mutex_unlock(&mtx);
}

void move_snake(SNAKE *snake, char ch1, DISPLAY *display)
{
  Coords tmp_body[20 * GAME_WIDTH];
  for (int i = 0; i < GAME_HEIGHT * GAME_WIDTH; i++)
  {
    tmp_body[i] = snake->body[i];
  }

  switch (ch1)
  {
  case 'w':
    if (snake->head.row - 1 < 0)
    {
      snake->head.row = field.g_height - 1;
    }
    else
    {
      snake->head.row -= 1;
    }
    snake->body[0].row = snake->head.row;
    break;
  case 'a':
    if (snake->head.col - 1 < 0)
    {
      snake->head.col = field.g_width - 1;
    }
    else
    {
      snake->head.col -= 1;
    }
    snake->body[0].col = snake->head.col;
    break;
  case 's':
    if (snake->head.row + 1 > field.g_height - 1)
    {
      snake->head.row = 0;
    }
    else
    {
      snake->head.row += 1;
    }
    snake->body[0].row = snake->head.row;
    break;
  case 'd':
    if (snake->head.col + 1 > field.g_width - 1)
    {
      snake->head.col = 0;
    }
    else
    {
      snake->head.col += 1;
    }
    snake->body[0].col = snake->head.col;
    break;
  default:
    break;
  }
  snake->direction = ch1;

  for (int i = 1; i < GAME_HEIGHT * GAME_WIDTH; i++)
  {
    snake->body[i] = tmp_body[i - 1];
  }
  check_obstacles(snake);
  check_food(snake, tmp_body, display);
}

void spawn_snake(Coords *start, SNAKE *snake)
{
  snake->head.row = start->row;
  snake->head.col = start->col;
  snake->lenght = 1;
  snake->body[0].col =start->col;
  snake->body[0].row =start->row;
  snake->direction = 'd';
  snake->is_spawned = true;
  snake->score = 0;
  copy_snake_to_grid(snake);
  info("snake was spawned\n");
}

bool grid_init(DISPLAY *display)
{
  field.g_width = GAME_WIDTH;
  field.g_height = GAME_HEIGHT;
  field.multiplication_factor = MULT_FACTOR;
  field.game_is_on = true;
  field.food_count = 0;
  for (int i = 0; i < field.g_height * field.g_width; i++)
  {
    field.grid[i] = 0;
  }

  printf("Gaming grid was inicialised with size: %d x %d\n", field.g_width, field.g_height);

  return 1;
}

void copy_snake_to_grid(SNAKE *snake)
{

  for (int i = 0; i < snake->lenght; i++)
  {
    field.grid[snake->body[i].row * field.g_width + snake->body[i].col] = 1;
  }
  printf("Snake head was copied to grid with coords: %d X, %d Y\n", snake->body[0].col, snake->body[0].row);
}

void copy_food_to_grid()
{
  for (int i = 0; i < SNAKE_FOOD; i++)
  {
    field.grid[field.food[i].row * field.g_width + field.food[i].col] = 2;
  }
  //field.grid[field.food.row*field.g_width+field.food.col] = 2;
}

void create_random_food(DISPLAY *display)
{
  int tmp_food_count = field.food_count;
  for (int i = 0; i < SNAKE_FOOD - tmp_food_count; i++)
  {
    int food = rand() % (field.g_width * field.g_height);
    if (field.grid[food] != 0)
    {
      while (field.grid[food] != 0)
      {
        food = rand() % (field.g_width * field.g_height);
      }
    }
    field.grid[food] = 2;

    field.food[i].row = food / field.g_width;
    field.food[i].col = food % field.g_width;
    field.food_count++;
    draw_grid_in_screen(&field, display);
    refresh_screen(display);
    usleep(10 * 1000);
  }
}

void create_random_food_index(int i)
{
  int food = rand() % (field.g_width * field.g_height);
  if (field.grid[food] != 0)
  {
    while (field.grid[food] != 0)
    {
      food = rand() % (field.g_width * field.g_height);
    }
  }
  field.grid[food] = 2;

  field.food[i].row = food / field.g_width;
  field.food[i].col = food % field.g_width;
}

void check_food(SNAKE *snake, Coords *tmp_body, DISPLAY *display)
{
  if (field.grid[snake->head.row * field.g_width + snake->head.col] == 2 /*field.food.row == snake->head.row && field.food.col == snake->head.col*/)
  {
    int pos = snake->lenght;
    snake->body[pos] = tmp_body[pos - 1];
    snake->lenght += 1;
    snake->score += 1;
    field.food_count -= 1;
    for (int i = 0; i < SNAKE_FOOD; i++)
    {
      if (field.food[i].row == snake->head.row && field.food[i].col == snake->head.col)
      {
        field.grid[snake->head.row * field.g_width + snake->head.col] = 0;
        create_random_food_index(i);
        led_food_up(display, false);
        
        pthread_mutex_lock(&mtx);
        global_data.food_led = true;
        pthread_cond_broadcast(&condvar);
        pthread_mutex_unlock(&mtx);
        copy_snake_to_grid(snake);
        break;
      }
    }
  }
}

void check_obstacles(SNAKE *snake)
{
  if (field.grid[snake->head.row * GAME_WIDTH + snake->head.col] == 1)
  {
    global_data.score = snake->score;
    field.game_is_on = false;
    info("GAME OVER - SNAKE DIED");
  }
}

void clean_grid(void)
{
  for (int i = 0; i < field.g_width * field.g_height; i++)
  {
    field.grid[i] = 0;
  }
}
