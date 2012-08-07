#include "canvaselement.h"

#include "../roboto/graphics/Canvas.h"
#include "../roboto/graphics/SystemCanvas.h"

#include "helper.h"
#include "../roboto/log.h"

/*
interface HTMLCanvasElement : HTMLElement {
           attribute unsigned long width; // default: 300
           attribute unsigned long height; // default: 150

  DOMString toDataURL(optional DOMString type, any... args);
  void toBlob(FileCallback? _callback, optional DOMString type, any... args);

  object? getContext(DOMString contextId, any... args);
};
*/

namespace roboto{
namespace js {

  static v8::Handle<v8::Value> v8getContext( const v8::Arguments& arguments ){
    v8::HandleScope scope;

    v8::Handle<v8::Value> val = v8::Undefined();
    // return 2d context
    
    return val;
  }

  
  Canvas::Canvas(v8::Handle<v8::Object>& that) : EventTarget(that) {
    this->canvas = SystemCanvas->getInstance()->createCanvas();
    
    V8_ADD_FUNCTION(that, getContext );
  }

  // TODO: Inherits Node
  V8_CREATE_CLASS(Canvas)
}}
