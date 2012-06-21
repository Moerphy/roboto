#ifndef ROBOTO_JS_WINDOW_H
#define ROBOTO_JS_WINDOW_H

#include "eventtarget.h"

#include <v8.h>

namespace roboto{
namespace js {
  
  static v8::Handle<v8::Value> scheduleTimer(const v8::Arguments& args, bool repeated );
  
  class Window : public EventTarget {
    private:


    public:
      Window(v8::Handle<v8::Object>&);

    
    public:
      static v8::Handle<v8::ObjectTemplate> New();
      static v8::Handle<v8::FunctionTemplate> getTemplate();
  };

}}

#endif
