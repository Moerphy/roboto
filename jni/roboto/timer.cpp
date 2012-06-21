#include "timer.h"

#include "log.h"

namespace roboto{
    int count = 0;
  
    struct TimedCallback{
      long target;
      void (*callback)( void* );
      void* data;
      
      TimedCallback* next;
    };
      
    TimedCallback* list;  
      
    void Timer::initialize(void* state){
      count = 0;
    }
    
    void Timer::update(){
      long time = getTime();
      TimedCallback* cb = list;
      int c = 0;
      while( cb != NULL && (time >= list->target) && (c < count) ){
        TimedCallback* cb = list;
        list = list->next;
        
        void (*func)( void* ) = cb->callback;
        (*func)( cb->data );
        
        delete cb;
        c++;
      }
      count -= c;
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
      count++;
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
