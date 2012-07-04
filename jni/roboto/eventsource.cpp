#include "eventsource.h"
#include "eventhandler.h"
#include "timer.h"
#include "runtime.h"

namespace roboto{
    
  /**
   * TODO: implement this stuff
   */
  static int32_t globalInputHandler(struct android_app* app, AInputEvent* event){
    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION){
      
      return 1;
    } else if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_KEY){
      
      return 1;
    }
    return 0;
  }

/**
 * TODO: implement this stuff
 */
  static void globalCmdHandler(struct android_app* app, int32_t cmd){
    Runtime* runt = static_cast<Runtime*>(app->userData);
    switch (cmd) {
      case APP_CMD_SAVE_STATE:
        // the OS asked us to save the state of the app
        app->destroyRequested = 1;
        break;
      case APP_CMD_INIT_WINDOW:
        // get the window ready for showing
        runt->onInitWindow();
        break;
      case APP_CMD_TERM_WINDOW:
        // clean up the window because it is being hidden/closed
        app->destroyRequested = 1;
        break;
      case APP_CMD_LOST_FOCUS:
        // if the app lost focus, avoid unnecessary processing (like monitoring the accelerometer)
        app->destroyRequested = 1;
        break;
      case APP_CMD_GAINED_FOCUS:
        // bring back a certain functionality, like monitoring the accelerometer
        break;
    }
  }
  
  
  
  void pollEvents(void* d){
    EventSource* ev = (EventSource*)d;
    // dispatch native event
    int ident;
    //int fdesc;
    int events;
    struct android_poll_source* source;
    
    while( (ident = ALooper_pollAll(0, NULL, &events, (void**)&source)) >= 0 ) {
      // process this event
      if( source != NULL ){
        source->process(ev->state, source);
      }
    }
    // update every 10ms
    Timer::timed( 10000, pollEvents, d);
  }
  
  EventSource::EventSource(android_app* s, EventHandler* listener) {
    // TODO network events
    this->state = s;
    this->state->onInputEvent = globalInputHandler;
    this->state->onAppCmd = globalCmdHandler;
    this->eventHandler = listener;
    
    pollEvents(this);
  }
}
