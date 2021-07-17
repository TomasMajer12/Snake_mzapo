//#define _POSIX_C_SOURCE 200112L

#include <setjmp.h>
#include "main.h"
#include "menu.h"
#include "display.h"
#include "menu_utils.h"
#include "serialize_lock.h"
#include "leds.h"

void color_menu(DISPLAY *display, font_descriptor_t *font_des)
{
  char *back = "BACK";
  char *colors = "SNAKE COLOR";
  char *set = "SET";
  int color_palet[] = {WHITE_COLOR, YELLOW_COLOR, GREEN_COLOR, LIGHT_GREY, BLUE_COLLOR};

  clear(display);
  draw_word(15, 10, colors, strlen(colors), 4, 0x7ff, display->fb, font_des, 40, display);
  draw_word(170, 190, set, strlen(set), 4, 0x7ff, display->fb, font_des, 40, display);
  draw_word(150, 250, back, strlen(back), 4, 0x7ff, display->fb, font_des, 40, display);
  menu_line(190, 250, display);
  refresh_screen(display);

  //color grid
  static int chosen = 0;
  draw_rectangle(25 + chosen * 90, 90, 70, 70, RED_COLOR, display);
  draw_rectangle(35, 100, 50, 50, WHITE_COLOR, display);
  draw_rectangle(125, 100, 50, 50, YELLOW_COLOR, display);
  draw_rectangle(215, 100, 50, 50, GREEN_COLOR, display);
  draw_rectangle(305, 100, 50, 50, LIGHT_GREY, display);
  draw_rectangle(395, 100, 50, 50, BLUE_COLLOR, display);
  char ch = ' ';
  int line = 1;
  while (true)
  {
    int r = read(0, &ch, 1);
    if (r == 0)
    {
      goto label;
    }

    if (r == 1)
    {
      if (ch == 'w')
      {
        line++;
        if (line > 1)
        {
          line = 0;
        }
      }
      if (ch == 's')
      {
        line--;
        if (line < 0)
        {
          line = 1;
        }
      }
      if (ch == 'd')
      {
        chosen++;
        if (chosen > 4)
        {
          chosen = 4;
        }
        clear_line(90, 160, display);
        draw_rectangle(25 + chosen * 90, 90, 70, 70, RED_COLOR, display);
        draw_rectangle(35, 100, 50, 50, WHITE_COLOR, display);
        draw_rectangle(125, 100, 50, 50, YELLOW_COLOR, display);
        draw_rectangle(215, 100, 50, 50, GREEN_COLOR, display);
        draw_rectangle(305, 100, 50, 50, LIGHT_GREY, display);
        draw_rectangle(395, 100, 50, 50, BLUE_COLLOR, display);
      }
      if (ch == 'a')
      {
        chosen--;
        if (chosen < 0)
        {
          chosen = 0;
        }
        clear_line(90, 160, display);
        draw_rectangle(25 + chosen * 90, 90, 70, 70, RED_COLOR, display);
        draw_rectangle(35, 100, 50, 50, WHITE_COLOR, display);
        draw_rectangle(125, 100, 50, 50, YELLOW_COLOR, display);
        draw_rectangle(215, 100, 50, 50, GREEN_COLOR, display);
        draw_rectangle(305, 100, 50, 50, LIGHT_GREY, display);
        draw_rectangle(395, 100, 50, 50, BLUE_COLLOR, display);
      }
      if (ch == '\n')
      {
        if (line == 0)
        {
          break;
        }
        if (line == 1)
        {
          info("Snake color was changed");
          global_data.color = color_palet[chosen];
          both_RGB_color(display, BLUE_LED);

          usleep(400 * 1000);

          both_RGB_color(display, OFF_LED);
        }
      }
    }
  label:
    switch (line)
    {
    case 1:
      clear_line_menu(display);
      menu_line(190, 250, display);
      refresh_screen(display);
      break;
    case 0:
      clear_line_menu(display);
      menu_line(250, 310, display);
      refresh_screen(display);
      break;
    }
  }
}

void speed_opt_menu(DISPLAY *display, font_descriptor_t *font_des)
{
  static int chosen = 1;
  jmp_buf buff;
  setjmp(buff);
  char *speed = "SPEED";
  char *quick = "QUICK";
  char *slow = "SLOW";
  char *set = "SET";
  char *back = "BACK";

  char time[5] = {0};
  sprintf(time, "%d", 50 * (chosen + 1));

  clear(display);
  draw_word(50, 30, speed, strlen(speed), 6, 0x7ff, display->fb, font_des, 60, display);
  draw_word(10, 178, quick, strlen(quick), 2, 0x7ff, display->fb, font_des, 20, display);
  draw_word(352, 178, slow, strlen(slow), 2, 0x7ff, display->fb, font_des, 20, display);
  draw_word(230, 178, time, strlen(time), 1, 0x7ff, display->fb, font_des, 10, display);
  draw_word(150, 190, set, strlen(set), 4, 0x7ff, display->fb, font_des, 40, display);
  draw_word(150, 250, back, strlen(back), 4, 0x7ff, display->fb, font_des, 40, display);

  //speed grid
  for (int i = 0; i < 19; i++)
  {
    draw_rectangle(20 + i * 22, 130, 16, 32, GREEN_COLOR, display);
  }
  draw_rectangle(20 + chosen * 22, 130, 16, 32, RED_COLOR, display);
  refresh_screen(display);

  char ch = ' ';
  int line = 1;
  while (true)
  {

    int r = read(0, &ch, 1);
    if (r == 0)
    {
      goto label;
    }

    if (r == 1)
    {
      if (ch == 'a')
      {
        chosen -= 1;
        if (chosen < 0)
        {
          chosen = 0;
        }
        printf("%d\n", chosen);
        longjmp(buff, 1);
      }
      if (ch == 'd')
      {
        chosen += 1;
        if (chosen > 18)
        {
          chosen = 18;
        }
        printf("%d\n", chosen);
        longjmp(buff, 1);
      }
      if (ch == 'w')
      {
        line++;
        if (line > 1)
        {
          line = 0;
        }
      }
      if (ch == 's')
      {
        line--;
        if (line < 0)
        {
          line = 1;
        }
      }
      if (ch == '\n')
      {
        if (line == 1)
        {
          global_data.refresh_time = 50 * (chosen + 1);
          printf("INFO: Game speed was changed - refresh time = %d\n", global_data.refresh_time);
          both_RGB_color(display, BLUE_LED);

          usleep(400 * 1000);

          both_RGB_color(display, OFF_LED);
        }
        if (line == 0)
        {
          break;
        }
      }
    }
  label:
    switch (line)
    {
    case 1:
      clear_line_menu(display);
      menu_line(190, 250, display);
      refresh_screen(display);
      break;
    case 0:
      clear_line_menu(display);
      menu_line(250, 310, display);
      refresh_screen(display);
      break;
    }
  }
}

void options_menu(DISPLAY *display, font_descriptor_t *font_des)
{
  jmp_buf buff;
  setjmp(buff);
  
  char *opt = "OPTIONS";
  char *speed = "SPEED";
  char *color = "COLOR";
  char *back = "BACK";

  clear(display);
  draw_word(50, 30, opt, strlen(opt), 6, 0x7ff, display->fb, font_des, 60, display);
  draw_word(50, 130, speed, strlen(speed), 4, 0x7ff, display->fb, font_des, 40, display);
  draw_word(50, 190, color, strlen(color), 4, 0x7ff, display->fb, font_des, 40, display);
  draw_word(50, 250, back, strlen(back), 4, 0x7ff, display->fb, font_des, 40, display);
  refresh_screen(display);

  char ch = ' ';
  int line = 2;
  while (true)
  {
    int r = read(0, &ch, 1);
    if (r == 0)
    {
      goto label;
    }
    if (r == 1)
    {
      if (ch == 'w')
      {
        line++;
        if (line > 2)
        {
          line = 0;
        }
      }
      if (ch == 's')
      {
        line--;
        if (line < 0)
        {
          line = 2;
        }
      }
      if (ch == '\n' /*&& line == 0*/)
      {
        if (line == 2)
        {
          speed_opt_menu(display, font_des);
          longjmp(buff, 1);
        }
        if (line == 1)
        {
          color_menu(display, font_des);
          longjmp(buff, 1);
        }
        else
        {
          break;
        }
      }
    }
  label:
    switch (line)
    {
    case 2:
      clear_line_menu(display);
      menu_line(130, 190, display);
      refresh_screen(display);
      break;

    case 1:
      clear_line_menu(display);
      menu_line(190, 250, display);
      refresh_screen(display);
      break;

    case 0:
      clear_line_menu(display);
      menu_line(250, 310, display);
      refresh_screen(display);
      break;
    }
  }
}

void menu(font_descriptor_t *font_des, DISPLAY *display)
{
  pthread_mutex_lock(&mtx);
  global_data.menu = true;
  pthread_cond_broadcast(&condvar);
  pthread_mutex_unlock(&mtx);
  
  clear(display);
  char *snake = "SNAKE";
  
  if (display->first_menu)
  { //prvni nacteni hry
    char *press = "PRESS ENTER TO";
    char *cont = "CONTINUE";

    draw_word(30, 40, snake, strlen(snake), 9, 0x7ff, display->fb, font_des, 90, display);
    draw_word(110, 200, press, strlen(press), 2, 0x7ff, display->fb, font_des, 20, display);
    draw_word(160, 245, cont, strlen(cont), 2, 0x7ff, display->fb, font_des, 20, display);
    refresh_screen(display);

    char ch = ' ';
    bool enter = true;
    while (enter)
    {
      int r = read(0, &ch, 1);
      if (r && ch == '\n')
      {
        enter = false;
        r = read(0, &ch, 1);
      }
    }
    display->first_menu = false;
  }
  if (display->menu)
  {
    jmp_buf buff;
    setjmp(buff);

    char *play = "PLAY";
    char *opt = "OPTIONS";
    char *q = "QUIT";

    clear(display);
    draw_word(50, 30, snake, strlen(snake), 6, 0x7ff, display->fb, font_des, 60, display);
    draw_word(50, 130, play, strlen(play), 4, 0x7ff, display->fb, font_des, 40, display);
    draw_word(50, 190, opt, strlen(opt), 4, 0x7ff, display->fb, font_des, 40, display);
    draw_word(50, 250, q, strlen(q), 4, 0x7ff, display->fb, font_des, 40, display);
    refresh_screen(display);

    char ch = ' ';
    int line = 2;
    while (true)
    {

      int r = read(0, &ch, 1);
      if (r == 0)
      {
        goto label;
      }

      if (r == 1)
      {
        if (ch == 'w')
        {
          line++;
          if (line > 2)
          {
            line = 0;
          }
        }
        if (ch == 's')
        {
          line--;
          if (line < 0)
          {
            line = 2;
          }
        }
        if (ch == '\n' && line == 2)
        {
          break;
        }
        if (ch == '\n' && line == 1)
        {
          options_menu(display, font_des);
          longjmp(buff, 1);
        }
        if (ch == '\n' && line == 0)
        {
          char *GoodBye = "GOODBYE";

          clear(display);
          draw_word(20, 100, GoodBye, strlen(GoodBye), 6, 0x7ff, display->fb, font_des, 60, display);
          refresh_screen(display);
          sleep(2);

          parlcd_write_cmd(display->parlcd_mem_base, 0x2c);
          for (display->ptr = 0; display->ptr < 480 * 320; display->ptr++)
          {
            parlcd_write_data(display->parlcd_mem_base, 0);
          }

          tcsetattr(STDIN_FILENO, TCSANOW, &display->oldt);

          pthread_mutex_lock(&mtx);
          global_data.menu = false;
          global_data.is_on = false;
          pthread_mutex_unlock(&mtx);
          
          pthread_join(global_data.threads[0], NULL);
          printf("Goodbye world\n");
          serialize_unlock();
          exit(0);
        }
      }
    label:
      switch (line)
      {
      case 2:
        clear_line_menu(display);
        menu_line(130, 190, display);
        refresh_screen(display);
        break;

      case 1:
        clear_line_menu(display);
        menu_line(190, 250, display);
        refresh_screen(display);
        break;
      case 0:
        clear_line_menu(display);
        menu_line(250, 310, display);
        refresh_screen(display);
        break;
      }
    }
  }
  menu_line(130, 190, display);
  clear(display);
  char *load = "LOADING...";
  draw_word(50, 100, load, strlen(load), 5, 0x7ff, display->fb, font_des, 45, display);
  refresh_screen(display);
  sleep(1);
}

void end_screen(DISPLAY *display, font_descriptor_t *font_des, jmp_buf game, jmp_buf jmp_menu, int score)
{
  pthread_mutex_lock(&mtx);
  global_data.menu = true;
  pthread_cond_broadcast(&condvar);
  pthread_mutex_unlock(&mtx);

  clear(display);

  char *game_over = "GAME OVER";
  char *main_menu = "MAIN MENU";
  char *sc = "SCORE:";
  char *play = "PLAY AGAIN";
  char *q = "QUIT";

  char sn_score[4] = {0};
  sprintf(sn_score, "%d", score);
  draw_word(210, 100, sn_score, 4, 3, RED_COLOR, display->fb, font_des, 30, display);
  draw_word(10, 10, game_over, strlen(game_over), 5, 0x7ff, display->fb, font_des, 50, display);
  draw_word(40, 100, sc, strlen(sc), 3, 0x7ff, display->fb, font_des, 30, display);
  draw_word(40, 150, play, strlen(play), 3, 0x7ff, display->fb, font_des, 30, display);
  draw_word(40, 200, main_menu, strlen(main_menu), 3, 0x7ff, display->fb, font_des, 30, display);
  draw_word(40, 250, q, strlen(q), 3, 0x7ff, display->fb, font_des, 30, display);
  refresh_screen(display);

  char ch = ' ';
  int line = 2;
  while (true)
  {
    int r = read(0, &ch, 1);

    if (r == 0)
    {
      goto jump;
    }
    if (r == 1)
    {
      if (ch == 'w')
      {
        line++;
        if (line > 2)
        {
          line = 0;
        }
      }
      if (ch == 's')
      {
        line--;
        if (line < 0)
        {
          line = 2;
        }
      }

      if (ch == '\n')
      {
        if (line == 0)
        {
          clear(display);
          char *GoodBye = "GOODBYE";
          draw_word(20, 100, GoodBye, strlen(GoodBye), 6, 0x7ff, display->fb, font_des, 60, display);
          refresh_screen(display);
          sleep(2);

          parlcd_write_cmd(display->parlcd_mem_base, 0x2c);
          for (display->ptr = 0; display->ptr < 480 * 320; display->ptr++)
          {
            parlcd_write_data(display->parlcd_mem_base, 0);
          }
          tcsetattr(STDIN_FILENO, TCSANOW, &display->oldt);

          pthread_mutex_lock(&mtx);
          global_data.menu = false;
          global_data.is_on = false;
          pthread_mutex_unlock(&mtx);

          pthread_join(global_data.threads[0], NULL);
          printf("Goodbye world\n");
          serialize_unlock();
          exit(0);
        }
        if (line == 1)
        {
          longjmp(jmp_menu, 1);
        }
        if (line == 2)
        {
          longjmp(game, 1);
        }
      }
    }
  jump:
    switch (line)
    {
    case 2:
      clear_line_menu(display);
      menu_line(150, 200, display);
      refresh_screen(display);
      break;

    case 1:
      clear_line_menu(display);
      menu_line(200, 250, display);
      refresh_screen(display);
      break;
    case 0:
      clear_line_menu(display);
      menu_line(250, 300, display);
      refresh_screen(display);
      break;
    }
  }
}

