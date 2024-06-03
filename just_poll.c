#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>

#include <pigpio.h>

/*
freq_count_1.c
2014-08-21
Public Domain

gcc -o freq_count_1 freq_count_1.c -lpigpio -lpthread
$ sudo ./freq_count_1  4 7 8

This program uses the gpioSetAlertFunc function to request
a callback (the same one) for each gpio to be monitored.

EXAMPLES

Monitor gpio 4 (default settings)
sudo ./freq_count_1  4

Monitor gpios 4 and 8 (default settings)
sudo ./freq_count_1  4 8

Monitor gpios 4 and 8, sample rate 2 microseconds
sudo ./freq_count_1  4 8 -s2

Monitor gpios 7 and 8, sample rate 4 microseconds, report every second
sudo ./freq_count_1  7 8 -s4 -r10

Monitor gpios 4,7, 8, 9, 10, 23 24, report five times a second
sudo ./freq_count_1  4 7 8 9 10 23 24 -r2

Monitor gpios 4, 7, 8, and 9, report once a second, sample rate 1us,
generate 2us edges (4us square wave, 250000 highs per second).
sudo ./freq_count_1  4 7 8 9 -r 10 -s 1 -p 2
*/

/*
times with minimal_clk on gpio 4 and 6
sudo ./freq1 4 6 -r10
 7%   0k   0k
 8%   5k   0k
 8%   5k   5k
 9%  10k   5k
 9%  10k  10k
10%  15k  10k
10%  15k  15k
10%  20k  15k
10%  20k  20k
11%  25k  20k
11%  25k  25k
11%  30k  30k
12%  40k  40k
13%  50k  50k
14%  60k  60k
16%  70k  70k
17%  80k  80k
18%  90k  90k
19% 100k 100k

*/

#define MAX_GPIOS 32

#define OPT_P_MIN 1
#define OPT_P_MAX 1000
#define OPT_P_DEF 20

#define OPT_R_MIN 1
#define OPT_R_MAX 300
#define OPT_R_DEF 10

#define OPT_S_MIN 1
#define OPT_S_MAX 10
#define OPT_S_DEF 5

typedef struct
{
   uint32_t first_tick;
   uint32_t last_tick;
   uint32_t pulse_count;
} gpioData_t;

static volatile gpioData_t g_gpio_data[MAX_GPIOS];
static volatile gpioData_t l_gpio_data[MAX_GPIOS];

static volatile int g_reset_counts[MAX_GPIOS];

static uint32_t g_mask;

static int g_num_gpios;
static int g_gpio[MAX_GPIOS];

static int g_opt_p = OPT_P_DEF;
static int refresh_rate = OPT_R_DEF;
static int sample_rate = OPT_S_DEF;
static int g_opt_t = 0;

void edges(int gpio, int level, uint32_t tick)
{
   l_gpio_data[gpio].last_tick = tick;

   if (level == 1) l_gpio_data[gpio].pulse_count++;

   if (g_reset_counts[gpio])
   {
      g_reset_counts[gpio] = 0;
      l_gpio_data[gpio].first_tick = tick;
      l_gpio_data[gpio].last_tick = tick;
      l_gpio_data[gpio].pulse_count = 0;
   }
}

int main(int argc, char *argv[])
{
   int i, rest, g, wave_id, mode, diff, tally;
   gpioPulse_t pulse[2];
   int count[MAX_GPIOS];

   /* command line parameters */


  /* monitor g_gpio level changes */

   for (i=0; i<g_num_gpios; i++) gpioSetAlertFunc(g_gpio[i], edges);

   mode = PI_INPUT;

   while (1)
   {
      gpioDelay(refresh_rate * 100000);

      for (i=0; i<g_num_gpios; i++)
      {
         g = g_gpio[i];
         g_gpio_data[g] = l_gpio_data[g];

         diff = g_gpio_data[g].last_tick - g_gpio_data[g].first_tick;
         tally = g_gpio_data[g].pulse_count;
         if (diff == 0) diff = 1;

         g_reset_counts[g] = 1;
         printf("g=%d %.0f (%d/%d)\n",
            g, 1000000.0 * tally / diff, tally, diff);
      }

   }

   gpioTerminate();
}

