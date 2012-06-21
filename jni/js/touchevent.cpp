#include "touchevent.h"
#include "event.h"
#include "helper.h"

namespace roboto{
namespace js{
  
  static v8::Handle<v8::Value> v8initEvent( const v8::Arguments& args ){
    return v8::Undefined();
  }
  
  
  TouchEvent::TouchEvent(v8::Handle<v8::Object>& that) : Event(that){
    // add functions
  }

  V8_CREATE_INHERITED_CLASS( TouchEvent, Event::getTemplate() );
}}
