/*******************************************************************
  Project main function template for MicroZed based MZ_APO board

  main.c      - main file

  Matyas Manur & Tomas Majer

 *******************************************************************/

#include "main.h"
#include "snake.h"
#include "display.h"
#include "serialize_lock.h"
#include "font_types.h"
#include "menu.h"
#include "leds.h"

DISPLAY display;

void global_data_init(void)
{
  global_data.refresh_time = REFRESH_TIME;
  global_data.color = WHITE_COLOR;
  global_data.menu = true;
  global_data.is_on = true;
  global_data.food_led = false;
  global_data.death_led = false;
}

void inicialization(void)
{
  display_init(&display);
  RGB1_init(&display);
  RGB2_init(&display);
  global_data_init();
  led_base_init(&display);
  pthread_mutex_init(&mtx, NULL);
  pthread_cond_init(&condvar, NULL);
}

int main(int argc, char *argv[])
{

  serialize_lock(1);

  inicialization();
  enum
  {
    LED_THREAD,
    NUM_THREADS
  };
  void *(*thr_threads[])(void *) = {led_thread};
 
  pthread_t threads[NUM_THREADS];
  global_data.threads = threads;
  pthread_create(&threads[0], NULL, thr_threads[0], &display);
  usleep(500*1000);
  
  font_descriptor_t *fdes = &font_winFreeSystem14x16;
  jmp_buf jmp_menu;
  jmp_buf game;

  setjmp(jmp_menu);
  menu(fdes, &display);
  printf("Hello world \n");

  setjmp(game);
  main_game(&display);

  end_screen(&display, fdes, game, jmp_menu, global_data.score);

  parlcd_write_cmd(display.parlcd_mem_base, 0x2c);
  for (display.ptr = 0; display.ptr < 480 * 320; display.ptr++)
  {
    parlcd_write_data(display.parlcd_mem_base, 0);
  }

  tcsetattr(STDIN_FILENO, TCSANOW, &display.oldt);

  
  pthread_mutex_lock(&mtx);
  global_data.menu = false;
  global_data.is_on = false;
  pthread_mutex_unlock(&mtx);

  pthread_join(threads[0], NULL);

  printf("Goodbye world\n");
  serialize_unlock();
  return 0;
}
