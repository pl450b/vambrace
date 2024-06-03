#include <stdio.h>
#include <stdlib.h>

#include <pigpio.h>

#define READ_PIN  13


typedef struct {
  uint32_t first_tick;
  unit32_t last_tick;
  unit32_t pulse_ct;
} gpioData_t;

static volatile gpioData_t g_data;
static volatile gpioData_t l_data;

static volatile int g_reset;

// gpio = pin number on raspberry pi to monitor
// level = which edge function was triggered on (1 = positive, 0 = negative)
// tick = timestamp when the trigger happens
void edge_trigger(into gpio, int level, uint32_t tick)
{
  l_data.last_tick = tick; // updates l_data with most recent timestamp
 
  if(level == 1) l_data.pulse_ct++; // if positive edge, increase pulse_ct
  
  // If a global reset triggered (g_reset = 1), set tick data to current timestamp
  if (g_reset) {
    l_data.first_tick = tick;
    l_data.last_tick = tick;
    l_data.pulse_ct = 0;
    g_reset = 0;
  }
}

int main(argc char *argv[]) {
  int diff; //timestamp difference
  int tally; // number pulses since last reset

  gpioSetAlertFuc(READ_PIN, edges) // Register callback function from pigpio library to edges function
  
  while(1) {
    gpioDelay(100000);

    g_data = l_data;

    diff = g_data.last_tick - g_data.first_tick;
    tally = g_data.pulse_ct;
    if(diff == 0) diff = 1; // Prevent dividing by zero

    g_reset = 1; // record new l_data
    printf("Pulse width: %f\n", tally / diff); // print pulse width by diving number of pulses by the time between two recordings
  }

  gpioTerminate();

}
