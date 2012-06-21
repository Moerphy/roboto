#ifndef ROBOTO_JS_EVENT_H
#define ROBOTO_JS_EVENT_H

#include <v8.h>

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
  
  
  class Event {
    private:
      v8::Persistent<v8::Value> v8val;
    public: 
      bool cancelled;
      bool bubbles;
      bool cancelable;
      char* type;
      long timeStamp;
      
      
    public:
      Event(v8::Handle<v8::Object>& that);
      
      void stopPropagation();
      void preventDefault();
      void initEvent(char* type, bool bubbles, bool cancelable);
      
      static v8::Handle<v8::FunctionTemplate> getTemplate();
  };
  
}}
#endif
