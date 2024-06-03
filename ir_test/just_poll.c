include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>

#include <pigpio.h>

#define READ_PIN  8

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

static volatile gpioData_t g_gpio_data;
static volatile gpioData_t l_gpio_data;

static volatile int g_reset_counts[MAX_GPIOS];

static uint32_t g_mask;

static int g_num_gpios;
static int g_opt_p = OPT_P_DEF;
static int refresh_rate = OPT_R_DEF;
static int sample_rate = OPT_S_DEF;
static int g_opt_t = 0;

void edges(int gpio, int level, uint32_t tick)
{
   l_gpio_data.last_tick = tick;

   if (level == 1) l_gpio_data.pulse_count++;

   if (g_reset_counts[gpio])
   {
      g_reset_counts[gpio] = 0;
      l_gpio_data.first_tick = tick;
      l_gpio_data.last_tick = tick;
      l_gpio_data.pulse_count = 0;
   }
}

int main(int argc, char *argv[])
{
   int i, rest, g, wave_id, mode, diff, tally;
   gpioPulse_t pulse[2];

  /* monitor g_gpio level changes */

   gpioSetAlertFunc(READ_PIN, edges);

   mode = PI_INPUT;

   while (1)
   {
      gpioDelay(refresh_rate * 100000);

	 g_gpio_data = l_gpio_data;

	 diff = g_gpio_data.last_tick - g_gpio_data.first_tick;
	 tally = g_gpio_data.pulse_count;
	 if (diff == 0) diff = 1;

	 g_reset_counts[g] = 1;
	 printf("g=%d %.0f (%d/%d)\n",
	    g, 1000000.0 * tally / diff, tally, diff);

   }

   gpioTerminate();
}

