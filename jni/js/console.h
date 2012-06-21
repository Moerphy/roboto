#ifndef ROBOTO_JS_CONSOLE_H
#define ROBOTO_JS_CONSOLE_H

#include <v8.h>
#include <android/log.h>

namespace roboto{
namespace js {
  
  class Console {
    public:
      Console();
      
      static v8::Handle<v8::FunctionTemplate> getTemplate();
  };

}}

#endif
