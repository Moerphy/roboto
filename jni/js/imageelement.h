#ifndef ROBOTO_JS_IMAGEELEMENT_H
#define ROBOTO_JS_IMAGEELEMENT_H

#include "eventtarget.h"
#include "documentevent.h"
#include "../roboto/graphics/texture.h"

#include <v8.h>

/*

*/

namespace roboto{
namespace js {
  class Image : public EventTarget {
    public:
      const char* src;
      int width;
      int height;
      Texture* texture;
      
    public:
      Image(v8::Handle<v8::Object>&);
      ~Image();
      
      void load();

      static v8::Handle<v8::FunctionTemplate> getTemplate();
  };

}}

#endif

