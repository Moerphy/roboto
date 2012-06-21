#ifndef ROBOTO_JS_DOCUMENTEVENT_H
#define ROBOTO_JS_DOCUMENTEVENT_H

#include "event.h"

#include <v8.h>

/**
interface DocumentEvent {
  Event              createEvent(in DOMString eventType)
                                        raises(DOMException);
};
*/

namespace roboto{
namespace js{
  //static v8::Handle<v8::Value> v8createEvent( const v8::Arguments& );
  
  class DocumentEvent {
    //friend v8::Handle<v8::Value> v8createEvent( const v8::Arguments& );
    private:
    
    public:
      DocumentEvent(v8::Handle<v8::Object>&);
      //Event* createEvent(char* eventType);
  };
  
}}

#endif
