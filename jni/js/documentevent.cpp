#include "documentevent.h"
#include "event.h"
#include "touchevent.h"
#include "helper.h"

#include "../roboto/log.h"

#include <string.h>
#include <v8.h>
/**
interface DocumentEvent {
  Event              createEvent(in DOMString eventType)
                                        raises(DOMException);
};
*/

namespace roboto{
namespace js{
  
  static v8::Handle<v8::FunctionTemplate> getEvent(char* eventType){
    if( strcmp(eventType, "TouchEvent") == 0 ){
      // Touch events, obviously
      return TouchEvent::getTemplate();
    }else if( strcmp(eventType, "Event") == 0 ){
      // DOMContentLoaded, load, unload, readystatechange
      return Event::getTemplate();
    } 
    // TODO: do we need any other event type?
    return v8::FunctionTemplate::New();
  }
  
  static v8::Handle<v8::Value> v8createEvent( const v8::Arguments& arguments ){
    v8::HandleScope scope;

    v8::Handle<v8::Value> val = v8::Undefined();
    
    if( arguments.Length() > 0 && arguments[0]->IsString() ){
      v8::Handle<v8::Value> docInternal = arguments.This()->GetInternalField( 0 );
      DocumentEvent* doc = static_cast<DocumentEvent*>( v8::Handle<v8::External>::Cast(docInternal)->Value() );
      
      v8::Handle<v8::String> type = arguments[0]->ToString();
      v8::String::AsciiValue typeAscii(type);
      
      v8::Handle<v8::FunctionTemplate> ev = getEvent(*typeAscii);
      val = ev->GetFunction()->NewInstance();
    }
    
    return val;
  }
  
  
  DocumentEvent::DocumentEvent(v8::Handle<v8::Object>& that){
    V8_ADD_FUNCTION( that, createEvent );
  }
  
  /*
  Event* DocumentEvent::createEvent(char* eventType){

  }
  */
}}
