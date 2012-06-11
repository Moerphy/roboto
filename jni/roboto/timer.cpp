#include "timer.h"

#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "robotoJS", __VA_ARGS__))

namespace roboto{
  
    struct TimedCallback{
      long target;
      void (*callback)( void* );
      void* data;
      
      TimedCallback* next;
    };
      
    TimedCallback* list;  
      
    void Timer::initialize(void* state){
    }
    
    void Timer::update(){
      long time = getTime();
      
      TimedCallback* cb = list;
      while( cb != NULL && (time >= cb->target) ){
        
        void (*func)( void* ) = cb->callback;
        (*func)( cb->data );

        TimedCallback* tmp = cb;
        cb = cb->next;
        delete tmp;
      }
      list = cb;
    }
    
    void Timer::addCallback( TimedCallback* cb ){
      if( list == NULL ){
        list = cb;
      }else{
        TimedCallback* last = NULL;
        TimedCallback* current = list;
        while( current != NULL && (current->target < cb->target) ){
          last = current;
          current = current->next;
        }
        if( last != NULL ){
          last->next = cb;
        }else{
          list = cb;
        }
        cb->next = current;
        
      }
    }
    
 
    void Timer::nextTick( void (*func)( void* ), void* data ){
      Timer::timed( 0, func, data );
    }
  
    void Timer::timed( long us, void (*func)( void* ), void* data ){
      TimedCallback* cb = new TimedCallback;
      cb->target = getTime() + us;
      cb->callback = func;
      cb->data = data;
      cb->next = NULL;
      
      addCallback(cb);
    }
  
}
