#ifndef ROBOTO_JS_CANVASELEMENT_H
#define ROBOTO_JS_CANVASELEMENT_H

#include "../roboto/graphics/Canvas.h"

#include "helper.h"
#include "../roboto/log.h"

/**

*/

namespace roboto{
namespace js{
  
  class CanvasElement : public EventTarget {
    private:
      Canvas* canvas;
    public:
      CanvasContext2D* getContext();
      
      static v8::Handle<v8::FunctionTemplate> getTemplate();
  };
  
}}

#endif
