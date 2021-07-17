#include "leds.h"
#include <unistd.h>

void *led_thread(void *displ)
{
    DISPLAY *display = (DISPLAY *)displ;
    while (global_data.is_on)
    {
        pthread_mutex_lock(&mtx);
        pthread_cond_wait(&condvar, &mtx);
        bool food_led = global_data.food_led;
        bool death_led = global_data.death_led;
        pthread_mutex_unlock(&mtx);
        if (food_led)
        {
            both_RGB_color(display, GREEN_LED);
            usleep(200 * 1000);
            both_RGB_color(display, OFF_LED);

            pthread_mutex_lock(&mtx);
            global_data.food_led = false;
            pthread_mutex_unlock(&mtx);
            continue;
        }
        else if (death_led)
        {
            both_RGB_color(display, RED_LED);
            usleep(800 * 1000);
            both_RGB_color(display, OFF_LED);
            usleep(800 * 1000);
            both_RGB_color(display, RED_LED);
            usleep(800 * 1000);
            both_RGB_color(display, OFF_LED);
            pthread_mutex_lock(&mtx);
            global_data.death_led = false;
            pthread_mutex_unlock(&mtx);
        }

        while (global_data.menu)

        {
            menu_led_animation(display);
        }
    }
    clear_led_line(display->led_stripe_mem_base);
    info("Led thread finished");
    return 0;
}

void led_base_init(DISPLAY *display)
{

    display->led_stripe_mem_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
    if (display->led_stripe_mem_base == NULL)
    {
        error("Led line init");
        exit(-1);
    }
}

void RGB1_init(DISPLAY *display)
{
    display->RGB1_mem_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
    if (display->RGB1_mem_base == NULL)
    {
        error("RGB1 init");
        exit(-1);
    }
}

void RGB2_init(DISPLAY *display)
{
    display->RGB2_mem_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
    if (display->RGB2_mem_base == NULL)
    {
        error("RGB2 init");
        exit(-1);
    }
}

void full_led_line(unsigned char *membase)
{
    pthread_mutex_lock(&mtx);
    volatile uint32_t *val_line = (volatile uint32_t *)(membase + SPILED_REG_LED_LINE_o);
    *val_line = 0xFFFFFFFF;
    pthread_mutex_unlock(&mtx);
}

void clear_led_line(unsigned char *membase)
{
    pthread_mutex_lock(&mtx);
    volatile uint32_t *val_line = (volatile uint32_t *)(membase + SPILED_REG_LED_LINE_o);
    *val_line = 0;
    pthread_mutex_unlock(&mtx);
}

void moving_led(unsigned char *membase, int delay)
{
    //uint32_t val_line = 2;
    pthread_mutex_lock(&mtx);
    volatile uint32_t *val_line = (volatile uint32_t *)(membase + SPILED_REG_LED_LINE_o);
    *val_line = 0x00000001;
    pthread_mutex_unlock(&mtx);
    uint32_t val = 0x80000000;
    uint32_t tmp = 0x10000000;
    //leds go from lef to right
    for (int i = 0; i < 30; i++)
    {
        val = (val >> 1) | tmp;
        pthread_mutex_lock(&mtx);
        *val_line = val;
        pthread_mutex_unlock(&mtx);
        tmp = val;
        usleep(delay * 1000);
    }
    pthread_mutex_lock(&mtx);
    val = 0x00000001;
    pthread_mutex_unlock(&mtx);
    tmp = 0x00000001;
    //leds go from right to left
    for (int i = 0; i < 30; i++)
    {
        val = (val << 1) | tmp;
        pthread_mutex_lock(&mtx);
        *val_line = ~val;
        pthread_mutex_unlock(&mtx);
        tmp = val;
        usleep(delay * 1000);
    }
    clear_led_line(membase);
}

void led_food_up(DISPLAY *display, bool reset)
{
    static uint32_t val;
    static uint32_t tmp;
    static int counter;
    volatile uint32_t *val_line = (volatile uint32_t *)(display->led_stripe_mem_base + SPILED_REG_LED_LINE_o);

    if (reset)
    {
        counter = 0;
        return;
    }
    if (!counter /*!(counter != NULL)*/)
    {
        counter = 1;
        tmp = 0x80000000;
        val = 0x80000000;
        *val_line = val;
        return;
    }

    val = (val >> 1) | tmp;
    if (counter == 31)
    {
        val = 0x00000001;
        tmp = 0x00000001;
        //leds go from right to left
        for (int i = 0; i < 30; i++)
        {
            val = (val << 1) | tmp;
            *val_line = ~val;
            tmp = val;
            info("led loop");
            usleep(5 * 1000);
        }
        counter = 1;
        tmp = 0x80000000;
        val = 0x80000000;
        *val_line = val;
        return;
    }

    *val_line = val;
    tmp = val;
    counter++;
}

void RGB1_color(DISPLAY *display, int color)
{
    pthread_mutex_lock(&mtx);
    volatile uint32_t *val_line = (volatile uint32_t *)(display->RGB1_mem_base + SPILED_REG_LED_RGB1_o);
    *val_line = color;
    pthread_mutex_unlock(&mtx);
}

void RGB2_color(DISPLAY *display, int color)
{
    pthread_mutex_lock(&mtx);
    volatile uint32_t *val_line = (volatile uint32_t *)(display->RGB2_mem_base + SPILED_REG_LED_RGB2_o);
    *val_line = color;
    pthread_mutex_unlock(&mtx);
}

void both_RGB_color(DISPLAY *display, int color)
{
    RGB1_color(display, color);
    RGB2_color(display, color);
}

void menu_led_animation(DISPLAY *display)
{
    uint32_t led_1 = 0xC0000000;
    uint32_t led_2 = 3;
    pthread_mutex_lock(&mtx);
    volatile uint32_t *val_line = (volatile uint32_t *)(display->led_stripe_mem_base + SPILED_REG_LED_LINE_o);
    *val_line = led_1 | led_2;
    bool go_on = global_data.menu;
    pthread_mutex_unlock(&mtx);
    for (int i = 0; i < 30; i++)
    {
        led_2 <<= 1;
        led_1 >>= 1;
        pthread_mutex_lock(&mtx);
        *val_line = led_1 | led_2;
        go_on = global_data.menu;
        pthread_mutex_unlock(&mtx);
        if (go_on == false)
        {
            return;
        }
        usleep(50 * 1000);
    }

    for (int i = 0; i < 30; i++)
    {
        led_2 >>= 1;
        led_1 <<= 1;
        pthread_mutex_lock(&mtx);
        *val_line = led_1 | led_2;
        go_on = global_data.menu;
        pthread_mutex_unlock(&mtx);
        if (go_on == false)
        {
            return;
        }
        usleep(50 * 1000);
    }
}