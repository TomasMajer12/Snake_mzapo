//#include "menu_utils.h"
//#include "main.h"
//#include "display.h"
#include "menu_utils.h"
#include "display.h"

int Get_char_width(int character, font_descriptor_t *font_de)
{
  int width = 0;
  if (!font_de->width)
  {
    width = font_de->maxwidth;
  }
  else
  {
    width = font_de->width[character - font_de->firstchar];
  }
  return width;
}

void draw_char(int x, int y, char ch, int scale, unsigned short color, unsigned short *fb, font_descriptor_t *fdes, DISPLAY *display)
{
  int w = Get_char_width(ch, fdes);
  const font_bits_t *ptr;

  if ((ch >= fdes->firstchar) && (ch - fdes->firstchar < fdes->size))
  {
    if (fdes->offset)
    {
      ptr = &fdes->bits[fdes->offset[ch - fdes->firstchar]];
    }
    else
    {
      int bw = (fdes->maxwidth + 15) / 16;
      ptr = &fdes->bits[(ch - fdes->firstchar) * bw * fdes->height];
    }
    int i, j;
    for (i = 0; i < fdes->height; i++)
    {
      font_bits_t val = *ptr;
      for (j = 0; j < w; j++)
      {
        if ((val & 0x8000) != 0)
        {
          draw_pixel_scale(x + scale * j, y + scale * i, scale, color, display);
          //printf("Pixel drawed, %d x %d color %d, scale %d , j %d, x %d\n", x+scale*j, y, color, scale, j, x);
        }
        val <<= 1;
      }
      ptr++;
    }
  }
}

void menu_line(int start, int end, DISPLAY *display)
{
  for (int x = 0; x < 480; x++)
  {
    for (int y = start; y < end; y++)
    {
      if (display->fb[x + 480 * y] == 0)
        draw_pixel(x, y, DARK_GREY, display);
    }
  }
}

void clear_line_menu(DISPLAY *display)
{
  for (int x = 0; x < 480; x++)
  {
    for (int y = 0; y < 320; y++)
    {
      if (display->fb[x + 480 * y] == DARK_GREY)
      {
        draw_pixel(x, y, 0, display);
      }
    }
  }
}

void draw_word(int x, int y, char *word, int length, int scale, unsigned short color, unsigned short *fb, font_descriptor_t *fdes, int space, DISPLAY *display)
{
  int I_ct = 0;

  for (int i = 0; i < length; i++)
  {
    if (i == 0)
    {
      draw_char(x, y, word[i], scale, color, fb, fdes, display);
    }
    else
    {

      draw_char(x + i * space - I_ct * 12, y, word[i], scale, color, fb, fdes, display);

      if (word[i] == 'I')
      {
        I_ct += 1;
      }
    }
  }
}

void clear(DISPLAY *display)
{
  for (display->ptr = 0; display->ptr < 480 * 320; display->ptr++)
  {
    display->fb[display->ptr] = 0;
  }
}

void draw_rectangle(int x, int y, int width, int height, unsigned short color, DISPLAY *display)
{
  for (int xi = 0; xi < width; xi++)
  {
    for (int yi = 0; yi < height; yi++)
    {
      draw_pixel(x + xi, y + yi, color, display);
    }
  }
}

void clear_line(int start, int end, DISPLAY *display)
{
  for (int x = 0; x < 480; x++)
  {
    for (int y = start; y < end; y++)
    {
      draw_pixel(x, y, 0, display);
    }
  }
}
