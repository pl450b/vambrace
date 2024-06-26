#include <stdio.h>
#include <stdlib.h>

#include <pigpio.h>

#define READ_PIN  13


typedef struct {
  uint32_t first_tick;
  uint32_t last_tick;
  uint32_t pulse_ct;
} gpioData_t;

volatile gpioData_t pulse_read;

static volatile int g_reset;

// gpio = pin number on raspberry pi to monitor
// level = which edge function was triggered on (1 = positive, 0 = negative)
// tick = timestamp when the trigger happens
void edge_trigger(int gpio, int level, uint32_t tick)
{
  pulse_read.last_tick = tick; // updates pulse_read with most recent timestamp
 
  // if(level == 1) pulse_read.pulse_ct++; // if positive edge, increase pulse_ct
  pulse_read.pulse_ct++;

  // If a global reset triggered (g_reset = 1), set tick data to current timestamp
  if (g_reset) {
    pulse_read.first_tick = tick;
    pulse_read.last_tick = tick;
    pulse_read.pulse_ct = 0;
    g_reset = 0;
  }
}

int main(int argc, char *argv[]) {
  int diff; //timestamp difference
  int tally; // number pulses since last reset
  gpioData_t temp_read;
	
  if (gpioInitialise() < 0) return 1;

  gpioSetAlertFunc(READ_PIN, edge_trigger); // Register callback function from pigpio library to edges function
  
  while(1) {
    gpioDelay(100000);

    temp_read = pulse_read; // make a temp copy so it can't change while we calc the pulse width
    

    diff = temp_read.last_tick - temp_read.first_tick;
    tally = temp_read.pulse_ct;
    if(diff == 0) diff = 1; // Prevent dividing by zero

    g_reset = 1; // record new pulse_read
    printf("Pulse width: %.0f\n", 1000000.0 * tally / diff); // print pulse width by diving number of pulses by the time between two recordings
  }

  gpioTerminate();

}
