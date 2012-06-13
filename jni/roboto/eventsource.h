#ifndef ROBOTO_EVENT_H
#define ROBOTO_EVENT_H

#include "eventhandler.h"
#include <android_native_app_glue.h>

/**
 * Handles the native events, calls back application when events occur
 */
namespace roboto{
 
  class EventSource{
    private:
      EventHandler* eventHandler;
      
    public:
      EventSource(struct android_app* s, EventHandler* listener);
      //~EventSource();
      android_app* state;
  };
  
}

#endif
