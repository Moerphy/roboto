#include "eventtarget.h"
#include "event.h"
#include "timer.h"

#include <v8.h>
#include <string.h>

#include "../roboto/log.h"

namespace roboto{
namespace js{
  
  struct EventDispatchItem{
    EventTarget* target;
    Event* ev;
  };
  /*
 // Introduced in DOM Level 2:
  interface EventTarget {
    void               addEventListener(in DOMString type, 
                                        in EventListener listener, 
                                        in boolean useCapture);
    boolean            dispatchEvent(in Event evt)
                                          raises(EventException);
  };
  */ // TODO
  static v8::Handle<v8::Value> v8addEventListener(  const v8::Arguments& arguments ){
    if( arguments.Length() > 2 ){
      v8::Local<v8::External> external = v8::Local<v8::External>::Cast( arguments.This()->GetInternalField(0) );
      EventTarget* object = static_cast<EventTarget*>( external->Value() );
      
      v8::Local<v8::String> typeStr = v8::Local<v8::String>::Cast(arguments[0]);
      v8::String::AsciiValue type(typeStr);
      
      v8::Local<v8::Function> cb = v8::Local<v8::Function>::Cast(arguments[1]);
      v8::Persistent<v8::Function> handler = v8::Persistent<v8::Function>::New(cb);

      object->addEventListener( *type, handler, false );
    }
    return v8::Undefined();
  }
  
  /**
  void removeEventListener(in DOMString type, in EventListener listener, in boolean useCapture)
  */
  static v8::Handle<v8::Value> v8removeEventListener(  const v8::Arguments& arguments ){
    // TODO: make event handler weak on removal
    
    if( arguments.Length() > 0 ){
      v8::Local<v8::External> external = v8::Local<v8::External>::Cast( arguments.This()->GetInternalField(0) );
      EventTarget* object = static_cast<EventTarget*>( external->Value() );
      
      // get event type to unbind
      v8::Handle<v8::Value> type = v8::Handle<v8::String>::Cast(arguments[0]);
      v8::String::AsciiValue typeAscii(type);
      
      // get function to unbind
      v8::Handle<v8::Value> listener = v8::Undefined();
      if( arguments.Length() > 1 ){
        listener =  v8::Handle<v8::Function>::Cast(arguments[1]);
      }
      
      // look for handlers to unbind
      EventItem* handler = object->eventList;
      EventItem* lastHandler = NULL;
      while( handler != NULL ){
        // strcmp == 0 -> equal
        if( (strcmp (handler->type, *typeAscii) == 0) && 
            ( listener->StrictEquals(v8::Undefined()) || listener->StrictEquals(handler->listener) ) ){
          EventItem* tmp = handler;
          if( lastHandler == NULL ){
            object->eventList = handler->next;
            handler = object->eventList;
          }else{
            lastHandler->next = handler->next;
            handler = lastHandler;
          }
          delete tmp;
          // remove handler
        }
        lastHandler = handler;
        handler = handler->next;
      }
    }
    
    return v8::Undefined();
  }
  static v8::Handle<v8::Value> v8dispatchEvent(  const v8::Arguments& arguments ){
    if( arguments.Length() > 0 ){
      v8::Local<v8::External> external = v8::Local<v8::External>::Cast( arguments.This()->GetInternalField(0) );
      EventTarget* object = static_cast<EventTarget*>( external->Value() );
      
      
      v8::Handle<v8::Object> ev =  v8::Handle<v8::Object>::Cast(arguments[0]);
      external = v8::Local<v8::External>::Cast( ev->GetInternalField(0) );
      Event* event = static_cast<Event*>( external->Value() );
      
      
      object->dispatchEvent(event, ev);
    }
    // TODO return value
    return v8::Undefined();
  }
  
  
  EventTarget::EventTarget(v8::Handle<v8::Object>& that){
    that->Set( v8::String::New("addEventListener"), v8::FunctionTemplate::New(v8addEventListener)->GetFunction() );
    that->Set( v8::String::New("removeEventListener"), v8::FunctionTemplate::New(v8removeEventListener)->GetFunction() );
    that->Set( v8::String::New("dispatchEvent"), v8::FunctionTemplate::New(v8dispatchEvent)->GetFunction() );
  }

  void EventTarget::addEventListener(char* type, EventListener listener, bool useCapture){
    EventItem* ev = new EventItem;
    ev->type = type;
    ev->listener = listener;
    ev->next = this->eventList;
    
    this->eventList = ev;
  }
  
  void EventTarget::removeEventListener(char* type, EventListener listener, bool useCapture){
    EventItem* ev = this->eventList;
    EventItem* last = NULL;
    while( ev != NULL ){
      if( ev->listener == listener ){
        break;
      }
      last = ev;
      ev = ev->next;
    }
    
    if( ev != NULL ){
      if( last == NULL ){
        this->eventList = ev->next;
      }else{
        last->next = ev->next;
      }
      delete ev;
    }
  }
  
  bool EventTarget::dispatchEvent(Event* event, v8::Handle<v8::Value> ev){
    EventItem* handler = this->eventList;
    
    while( handler != NULL && event != NULL ){
      // strcmp == 0 -> equal
      if( handler->type != NULL && event->type != NULL && strcmp (handler->type, event->type) == 0 ){
        // TODO: Event arguments
        handler->listener->Call( v8::Context::GetCurrent()->Global(), 1, &ev );
        
        if( event->cancelled ){
          break;
        }
      }
      handler = handler->next;
    }

    return true;
  }
}}
