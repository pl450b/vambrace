#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <pigpio.h>

#define READ_PIN  13

// States
#define SLEEP   0
#define AWAKE   1

#define WAKE_THRESHOLD  150   // result value considered NULL
#define SLEEP_THRESHOLD 50    // number of loops before system sleeps again

#define FIFO_PATH "/tmp/gesture_fifo"

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
  gpioData_t temp_read = pulse_read; // make a temp copy so it can't change while we calc the pulse width
  
  diff = temp_read.last_tick - temp_read.first_tick;
  tally = temp_read.pulse_ct;
  if(diff == 0) diff = 1; // Prevent dividing by zero
  
  g_reset = 1; // record new pulse_read
  return(1000000 * tally / diff);
}

void print_lines(int number) {
  printf("Result: ");
  int dash_ct = number / 2;
  for(int i = 0; i < dash_ct; i++) {
    printf("-");
  }
  printf("\n");
}

// Open the given FIFo, print result into it, then close the FIFO
void print_to_fifo(const char* fifo, int input) {
  char num_str[4];
  sprintf(num_str, "%i", input);
  int fifo_num = open(fifo, O_WRONLY);
  write(fifo_num, num_str, 4);
  close(fifo_num);
}


int main(int argc, char *argv[]) {
  int result;
  int state = SLEEP;
  int awake_ct = 0;
  
  printf("Test\n");

  const char *fifo = FIFO_PATH;
  mkfifo(fifo, 0666);
  
  printf("[INFO] FIFO established\n");

  if (gpioInitialise() < 0) {
    printf("[ERROR] GPIO setup failure\n");
    return -1;
  }

  printf("[INFO] GPIO setup success\n");

  gpioSetAlertFunc(READ_PIN, edge_trigger); // Register callback function from pigpio library to edges function
  
  printf("[INFO] Starting read loop\n");
  while(1) {
    gpioDelay(100000);
    result = read_frequency();
    switch(state) 
    {
    case SLEEP:
         if(result < WAKE_THRESHOLD) state = AWAKE;
         break;
    case AWAKE:
         print_lines(result); 
         print_to_fifo(fifo, result);

         if(result < WAKE_THRESHOLD)
            awake_ct = 0;
         else 
            awake_ct++;

         if(awake_ct >= SLEEP_THRESHOLD) {
            state = SLEEP;
            awake_ct = 0; 
            printf("SLEEP\n");
            print_to_fifo(fifo, 999);
          }
          break;
    default:
         state = SLEEP;
    }
  }

  gpioTerminate();

}
