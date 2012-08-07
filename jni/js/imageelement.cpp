#include "imageelement.h"

#include "helper.h"
#include "../roboto/log.h"
#include "eventtarget.h"
#include "documentevent.h"

#include "../roboto/graphics/texture.h"
#include "../roboto/timer.h"

#include <v8.h>

/*

*/

namespace roboto{
namespace js {
  
  /*
   * V8 wrapper methods
   */


  /**
   * Accessors for the src attribute, this starts the loading of the image data.
   */
  v8::Handle<v8::Value> v8getsrc(v8::Local<v8::String> property, const v8::AccessorInfo &info) {
    V8_UNWRAP_INTERNAL( info.Holder(), Image, img );

    return v8::String::New( img->src );
  }
  
  void v8setsrc(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::AccessorInfo& info) {
    V8_UNWRAP_INTERNAL( info.Holder(), Image, img );
    
    v8::String::AsciiValue srcCStr( value->ToString() );
    
    img->src = *srcCStr;
    img->load();
  }
  
  
  /**
   * Accessors for the width attribute, this starts the loading of the image data.
   */
  v8::Handle<v8::Value> v8getwidth(v8::Local<v8::String> property, const v8::AccessorInfo &info) {
    V8_UNWRAP_INTERNAL( info.Holder(), Image, img );

    return v8::Integer::New( img->width );
  }
  
  void v8setwidth(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::AccessorInfo& info) {
    V8_UNWRAP_INTERNAL( info.Holder(), Image, img );

    img->width = value->Int32Value();
  }
  
  /**
   * Accessors for the width attribute, this starts the loading of the image data.
   */
  v8::Handle<v8::Value> v8getheight(v8::Local<v8::String> property, const v8::AccessorInfo &info) {
    V8_UNWRAP_INTERNAL( info.Holder(), Image, img );

    return v8::Integer::New( img->height );
  }
  
  void v8setheight(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::AccessorInfo& info) {
    V8_UNWRAP_INTERNAL( info.Holder(), Image, img );

    img->height = value->Int32Value();
  }
  
  
  /*
   * Image methods
   */
  void dispatchLoadedCallback( void* data ){
    
  }
  
  Image::Image(v8::Handle<v8::Object>& that) : EventTarget(that){
    this->texture = NULL;
    // TODO check how to get this on the objecttemplate best
    
    V8_ADD_ACCESSOR(that, src );
    V8_ADD_ACCESSOR(that, width );
    V8_ADD_ACCESSOR(that, height );
  }
  
  Image::~Image(){
    this->texture->dispose();
    delete this->texture;
  }
  
  void Image::load(){
    const char* src = this->src;
    if( src != NULL ){
      this->texture = Texture::load(src);
      Timer::nextTick( dispatchLoadedCallback, this );
    }
  }
  
  

  // TODO: Inherits Node
  V8_CREATE_CLASS(Image)
}}
