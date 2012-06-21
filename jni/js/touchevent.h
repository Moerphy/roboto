#ifndef ROBOTO_JS_TOUCHEVENT_H
#define ROBOTO_JS_TOUCHEVENT_H

#include <v8.h>
#include "event.h"

namespace roboto{
namespace js{
  
  /*
    interface Event {

      // PhaseType
      const unsigned short      CAPTURING_PHASE                = 1;
      const unsigned short      AT_TARGET                      = 2;
      const unsigned short      BUBBLING_PHASE                 = 3;

      readonly attribute DOMString        type;
      readonly attribute EventTarget      target;
      readonly attribute EventTarget      currentTarget;
      readonly attribute unsigned short   eventPhase;
      readonly attribute boolean          bubbles;
      readonly attribute boolean          cancelable;
      readonly attribute DOMTimeStamp     timeStamp;
      void               stopPropagation();
      void               preventDefault();
      void               initEvent(in DOMString eventTypeArg, 
                                   in boolean canBubbleArg, 
                                   in boolean cancelableArg);
    };
  */
  
  
  class TouchEvent : public Event {
    public:
      TouchEvent(v8::Handle<v8::Object>& that);
      
      static v8::Handle<v8::FunctionTemplate> getTemplate();
  };
  
}}
#endif
