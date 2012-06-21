#include "event.h"
#include "helper.h"
#include "../roboto/log.h"

namespace roboto{
namespace js{
  
  static v8::Handle<v8::Value> v8initEvent( const v8::Arguments& arguments ){
    if( arguments.Length() > 1 ){
      v8::Handle<v8::Value> eventInternal = arguments.This()->GetInternalField( 0 );
      Event* ev = static_cast<Event*>( v8::Handle<v8::External>::Cast(eventInternal)->Value() );
      
      v8::String::AsciiValue type(v8::Handle<v8::String>::Cast(arguments[0]) );
      
      ev->initEvent( *type, false, false);
    }
    return v8::Undefined();
  }
  
  
  Event::Event(v8::Handle<v8::Object>& that){
    this->cancelled = false;
    this->cancelable = true;
    this->bubbles = true;
    
    
    V8_ADD_FUNCTION( that, initEvent );
  }
  
  
  void Event::stopPropagation(){
  }
  void Event::preventDefault(){
    
  }
  void Event::initEvent(char* type, bool bubbles, bool cancelable){
    this->type = type;
    this->bubbles = bubbles;
    this->cancelable = cancelable;
  }
  
  V8_CREATE_CLASS( Event );
}}
