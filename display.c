#include "display.h"
#include "menu.h"
#include "menu_utils.h"

#define D_WIDTH 480
#define D_HEIG 320

void display_init(DISPLAY *display)
{
  int i, j;
  unsigned int c;
  display->fb = (unsigned short *)malloc(320 * 480 * 2);
  tcgetattr(STDIN_FILENO, &display->oldt);
  display->newt = display->oldt;

  //menu bools
  display->first_menu = true;
  display->menu = true;
  //display->led_start = true;

  //ICANON normally takes care that one line at a time will be processed
  //that means it will return if it sees a "\n" or an EOF or an EOL
  display->newt.c_lflag &= ~(ICANON);
  display->newt.c_cc[VMIN] = 0; // bytes until read unblocks.
  display->newt.c_cc[VTIME] = 0;
  //Those new settings will be set to STDIN

  //TCSANOW tells tcsetattr to change attributes immediately.
  tcsetattr(STDIN_FILENO, TCSANOW, &display->newt);
  display->parlcd_mem_base = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);

  if (display->parlcd_mem_base == NULL)
  {
    printf("ERROR\n");
    exit(1);
  }

  parlcd_hx8357_init(display->parlcd_mem_base);
  parlcd_write_cmd(display->parlcd_mem_base, 0x2c);
  display->ptr = 0;
  for (i = 0; i < 320; i++)
  {
    for (j = 0; j < 480; j++)
    {
      c = 0;
      display->fb[display->ptr] = c;
      parlcd_write_data(display->parlcd_mem_base, display->fb[display->ptr++]);
    }
  }
}

void draw_pixel(int x, int y, unsigned short color, DISPLAY *display)
{
  if (x >= 0 && x < 480 && y >= 0 && y < 320)
  {
    display->fb[x + 480 * y] = color;
  }
}

void draw_pixel_scale(int x, int y, int scale, unsigned short color, DISPLAY *display)
{
  int i, j;
  for (i = 0; i < scale; i++)
  {
    for (j = 0; j < scale; j++)
    {
      draw_pixel(x + i, y + j, color, display);
    }
  }
}

void refresh_screen(DISPLAY *display)
{
  parlcd_write_cmd(display->parlcd_mem_base, 0x2c);
  for (display->ptr = 0; display->ptr < 480 * 320; display->ptr++)
  {
    parlcd_write_data(display->parlcd_mem_base, display->fb[display->ptr]);
  }
}

void draw_grid_in_screen(GAME_GRID *field, DISPLAY *display)
{
  int row, col;
  for (int i = 0; i < field->g_width * field->g_height; i++)
  {
    if (field->grid[i] == 1)
    {
      row = (i / field->g_width) * MULT_FACTOR + 4 * 16 + 8;
      col = (i % field->g_width) * MULT_FACTOR + 8;
      for (int x = 1; x < MULT_FACTOR - 1; x++)
      {
        for (int y = 1; y < MULT_FACTOR - 1; y++)
        {
          draw_pixel(col + x, row + y, global_data.color, display);
        }
      }
    }
    else if (field->grid[i] == 2)
    {
      row = (i / field->g_width) * MULT_FACTOR + 4 * 16 + 8;
      col = (i % field->g_width) * MULT_FACTOR + 8;
      for (int x = 0; x < MULT_FACTOR; x++)
      {
        for (int y = 0; y < MULT_FACTOR; y++)
        {
          draw_pixel(col + x, row + y, RED_COLOR, display);
        }
      }
    }
  }
}

void game_mask(DISPLAY *display, GAME_GRID *field, SNAKE *snake)
{
  font_descriptor_t *fdes = &font_winFreeSystem14x16;
  int offset = 25;
  int scale = 2;

  char sn_score[4] = {0};
  sprintf(sn_score, "%d", snake->score);

  char time[4] = {0};
  sprintf(time, "%d", field->time);

  draw_word(270, 10, "TIME:", 5, scale, WHITE_COLOR, display->fb, fdes, offset, display);
  draw_word(400, 10, time, 4, scale, RED_COLOR, display->fb, fdes, offset, display);

  draw_word(10, 10, "SCORE:", 6, scale, WHITE_COLOR, display->fb, fdes, offset, display);
  draw_word(180, 10, sn_score, 4, scale, RED_COLOR, display->fb, fdes, offset, display);

  for (int y = 0; y < D_HEIG; y++)
  {
    draw_pixel(6, y, BLUE_COLLOR, display);
    draw_pixel(D_WIDTH - 6, y, BLUE_COLLOR, display);
  }
  
  for (int x = 0; x < D_WIDTH; x++)
  {
    draw_pixel(x, 0, BLUE_COLLOR, display);
    draw_pixel(x, 4 * 16 + 8, BLUE_COLLOR, display);
    draw_pixel(x, D_HEIG - 6, BLUE_COLLOR, display);
  }
}
