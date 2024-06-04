#include <stdio.h>
#include <stdlib.h>

#include <pigpio.h>

#define READ_PIN  13

// States
#define SLEEP   0
#define AWAKE   1

#define WAKE_THRESHOLD  150   // result value considered NULL
#define SLEEP_THRESHOLD 50    // number of loops before system sleeps again

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

int read_frequency(void)
{
  int diff; // timestamp difference
  int tally; // pulse number since last read
  gpioDelay temp_read = pulse_read; // make a temp copy so it can't change while we calc the pulse width
  
  diff = temp_read.last_tick - temp_read.first_tick;
  tally = temp_read.pulse_ct;
  if(diff == 0) diff = 1; // Prevent dividing by zero
  
  g_reset = 1; // record new pulse_read
  return(1000000 * tally / diff);
}

int main(int argc, char *argv[]) {
  int result;
  int state = SLEEP;
  int awake_ct = 0;
	
  if (gpioInitialise() < 0) return 1;

  gpioSetAlertFunc(READ_PIN, edge_trigger); // Register callback function from pigpio library to edges function
  
  while(1) {
    gpioDelay(100000);
    result = read_frequency();
    case(state) 
    {
    SLEEP:
         if(result < WAKE_THRESHOLD) state = AWAKE;
         break;
    AWAKE:
         printf("Result: %i\n", result);
         if(result < WAKE_THRESHOLD)
            awake_ct = 0;
         else 
            awake_ct++;

         if(awake_ct >= SLEEP_THRESHOLD) {
            state = SLEEP;
            awake_ct = 0;           
          }
          break;
    default:
         state = SLEEP;
    }
  }

  gpioTerminate();

}
