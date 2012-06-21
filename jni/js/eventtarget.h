#ifndef ROBOTO_JS_EVENTTARGET_H
#define ROBOTO_JS_EVENTTARGET_H

#include "./event.h"

#include <v8.h>

namespace roboto{
namespace js{
  
  typedef v8::Persistent<v8::Function> EventListener;
  
  struct EventItem{
    char* type;
    EventListener listener;
    EventItem* next;
  };
  
  /*
 // Introduced in DOM Level 2:
  interface EventTarget {
    void               addEventListener(in DOMString type, 
                                        in EventListener listener, 
                                        in boolean useCapture);
    void               removeEventListener(in DOMString type, 
                                           in EventListener listener, 
                                           in boolean useCapture);
    boolean            dispatchEvent(in Event evt)
                                          raises(EventException);
  };
  */
  static v8::Handle<v8::Value> v8removeEventListener(const v8::Arguments&);
  
  class EventTarget {
    friend v8::Handle<v8::Value> v8removeEventListener(const v8::Arguments&);
    private:
      EventItem* eventList;
    public:
      EventTarget(v8::Handle<v8::Object>&);
      void addEventListener(char* type, EventListener listener, bool useCapture);
      void removeEventListener(char* type, EventListener listener, bool useCapture);
      bool dispatchEvent(Event* evt, v8::Handle<v8::Value>);
  };
  
}}
#endif
