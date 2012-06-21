#ifndef ROBOTO_TIMER_H
#define ROBOTO_TIMER_H

#include <time.h>

namespace roboto{
  
  inline long getTime(){
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return now.tv_sec*1000000 + now.tv_nsec/1000;
  }
  
  struct TimedCallback;
  
  class Timer{
    private:
      static void addCallback(TimedCallback*);
    
    public:
      static void initialize(void*);
      static void update();
      
      /**
       * Schedule callback to be called on the next event loop update.
       * Equivalent to Timer::timed(0, func, d);
       * @param func callback function to call
       * @param d data to pass into callback
       */
      static void nextTick( void (*func)( void* ), void* d );
      /**
       * Schedule callback to be called after a set time.
       * @param us number of microseconds to wait until executing this callback
       * @param func callback function to call
       * @param d data to pass into callback
       */
      static void timed( long us, void (*func)( void* ), void* d );
  };
  
}

#endif
