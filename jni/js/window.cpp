#include "window.h"
#include "document.h"
#include "console.h"

#include "helper.h"

//#include "../roboto/runtime.h"
#include "../roboto/timer.h"
#include "../roboto/log.h"

#include <v8.h>



/*
[NamedPropertiesObject]
interface Window : EventTarget {
  // the current browsing context
  [Unforgeable] readonly attribute WindowProxy window;
  [Replaceable] readonly attribute WindowProxy self;
  [Unforgeable] readonly attribute Document document;
           attribute DOMString name; 
  [PutForwards=href, Unforgeable] readonly attribute Location location;
  readonly attribute History history;

  boolean find(optional DOMString aString, optional boolean aCaseSensitive, optional boolean aBackwards, optional boolean aWrapAround, optional boolean aWholeWord, optional boolean aSearchInFrames, optional boolean aShowDialog);

  [Replaceable] readonly attribute BarProp locationbar;
  [Replaceable] readonly attribute BarProp menubar;
  [Replaceable] readonly attribute BarProp personalbar;
  [Replaceable] readonly attribute BarProp scrollbars;
  [Replaceable] readonly attribute BarProp statusbar;
  [Replaceable] readonly attribute BarProp toolbar;
           attribute DOMString status;
  void close();
  void stop();
  void focus();
  void blur();

  // other browsing contexts
  [Replaceable] readonly attribute WindowProxy frames;
  [Replaceable] readonly attribute unsigned long length;
  [Unforgeable] readonly attribute WindowProxy top;
           attribute WindowProxy? opener;
  readonly attribute WindowProxy parent;
  readonly attribute Element? frameElement;
  WindowProxy open(optional DOMString url, optional DOMString target, optional DOMString features, optional boolean replace);
  getter WindowProxy (unsigned long index);
  getter object (DOMString name);

  // the user agent
  readonly attribute Navigator navigator; 
  readonly attribute External external;
  readonly attribute ApplicationCache applicationCache;

  // user prompts
  void alert(DOMString message);
  boolean confirm(DOMString message);
  DOMString? prompt(DOMString message, optional DOMString default);
  void print();
  any showModalDialog(DOMString url, optional any argument);

  // cross-document messaging
  void postMessage(any message, DOMString targetOrigin, optional sequence<Transferable> transfer);

  // event handler IDL attributes
  [TreatNonCallableAsNull] attribute Function? onabort;
  [TreatNonCallableAsNull] attribute Function? onafterprint;
  [TreatNonCallableAsNull] attribute Function? onbeforeprint;
  [TreatNonCallableAsNull] attribute Function? onbeforeunload;
  [TreatNonCallableAsNull] attribute Function? onblur;
  [TreatNonCallableAsNull] attribute Function? oncancel;
  [TreatNonCallableAsNull] attribute Function? oncanplay;
  [TreatNonCallableAsNull] attribute Function? oncanplaythrough;
  [TreatNonCallableAsNull] attribute Function? onchange;
  [TreatNonCallableAsNull] attribute Function? onclick;
  [TreatNonCallableAsNull] attribute Function? onclose;
  [TreatNonCallableAsNull] attribute Function? oncontextmenu;
  [TreatNonCallableAsNull] attribute Function? oncuechange;
  [TreatNonCallableAsNull] attribute Function? ondblclick;
  [TreatNonCallableAsNull] attribute Function? ondrag;
  [TreatNonCallableAsNull] attribute Function? ondragend;
  [TreatNonCallableAsNull] attribute Function? ondragenter;
  [TreatNonCallableAsNull] attribute Function? ondragleave;
  [TreatNonCallableAsNull] attribute Function? ondragover;
  [TreatNonCallableAsNull] attribute Function? ondragstart;
  [TreatNonCallableAsNull] attribute Function? ondrop;
  [TreatNonCallableAsNull] attribute Function? ondurationchange;
  [TreatNonCallableAsNull] attribute Function? onemptied;
  [TreatNonCallableAsNull] attribute Function? onended;
  [TreatNonCallableAsNull] attribute Function? onerror;
  [TreatNonCallableAsNull] attribute Function? onfocus;
  [TreatNonCallableAsNull] attribute Function? onhashchange;
  [TreatNonCallableAsNull] attribute Function? oninput;
  [TreatNonCallableAsNull] attribute Function? oninvalid;
  [TreatNonCallableAsNull] attribute Function? onkeydown;
  [TreatNonCallableAsNull] attribute Function? onkeypress;
  [TreatNonCallableAsNull] attribute Function? onkeyup;
  [TreatNonCallableAsNull] attribute Function? onload;
  [TreatNonCallableAsNull] attribute Function? onloadeddata;
  [TreatNonCallableAsNull] attribute Function? onloadedmetadata;
  [TreatNonCallableAsNull] attribute Function? onloadstart;
  [TreatNonCallableAsNull] attribute Function? onmessage;
  [TreatNonCallableAsNull] attribute Function? onmousedown;
  [TreatNonCallableAsNull] attribute Function? onmousemove;
  [TreatNonCallableAsNull] attribute Function? onmouseout;
  [TreatNonCallableAsNull] attribute Function? onmouseover;
  [TreatNonCallableAsNull] attribute Function? onmouseup;
  [TreatNonCallableAsNull] attribute Function? onmousewheel;
  [TreatNonCallableAsNull] attribute Function? onoffline;
  [TreatNonCallableAsNull] attribute Function? ononline;
  [TreatNonCallableAsNull] attribute Function? onpause;
  [TreatNonCallableAsNull] attribute Function? onplay;
  [TreatNonCallableAsNull] attribute Function? onplaying;
  [TreatNonCallableAsNull] attribute Function? onpagehide;
  [TreatNonCallableAsNull] attribute Function? onpageshow;
  [TreatNonCallableAsNull] attribute Function? onpopstate;
  [TreatNonCallableAsNull] attribute Function? onprogress;
  [TreatNonCallableAsNull] attribute Function? onratechange;
  [TreatNonCallableAsNull] attribute Function? onreset;
  [TreatNonCallableAsNull] attribute Function? onresize;
  [TreatNonCallableAsNull] attribute Function? onscroll;
  [TreatNonCallableAsNull] attribute Function? onseeked;
  [TreatNonCallableAsNull] attribute Function? onseeking;
  [TreatNonCallableAsNull] attribute Function? onselect;
  [TreatNonCallableAsNull] attribute Function? onshow;
  [TreatNonCallableAsNull] attribute Function? onstalled;
  [TreatNonCallableAsNull] attribute Function? onstorage;
  [TreatNonCallableAsNull] attribute Function? onsubmit;
  [TreatNonCallableAsNull] attribute Function? onsuspend;
  [TreatNonCallableAsNull] attribute Function? ontimeupdate;
  [TreatNonCallableAsNull] attribute Function? onunload;
  [TreatNonCallableAsNull] attribute Function? onvolumechange;
  [TreatNonCallableAsNull] attribute Function? onwaiting;
};
 */


namespace roboto{
namespace js {

 
  
  struct TimerCallbackData{
    v8::Persistent<v8::Function> handler;
    int timeout;
  };

  /*
  Window Interface, Timer Attributes
  // should timers allow more than long, maybe a floating point type?
  // don't think anyone's timers have more precision
  */
  void executeTimedCallback(void* d){
    v8::HandleScope handlescope;

    TimerCallbackData*  data = static_cast<struct TimerCallbackData*>(d);
    //const v8::Arguments* args = data->args;

    v8::Local<v8::Object> globalObj = v8::Context::GetCurrent()->Global();
    v8::Persistent<v8::Function> handler = data->handler;

    handler->Call( globalObj, 0, NULL );
    
    if( data->timeout > 0 ){
      // TODO
       ::roboto::Timer::timed( data->timeout*1000, executeTimedCallback, data );
    }else{
      handler.Dispose();
      delete data;
    }
  }
  
  static v8::Handle<v8::Value> scheduleTimer( const v8::Arguments& args, bool repeated ){
    v8::HandleScope handlescope;
    
    if( args.Length() > 0 && args[0]->IsFunction() ){ 
      // disposed in executeTimedCallback
      TimerCallbackData* data = new TimerCallbackData;
      
      // default timeout is 4ms
      int timeout = 4;
      if( args.Length() > 1 && args[1]->IsNumber() ){
        timeout = (int) args[1]->ToNumber()->Value();
        if( timeout < 4 ){
          timeout = 4;
        }
      }
      
      v8::Local<v8::Function> cb = v8::Local<v8::Function>::Cast(args[0]);
      v8::Persistent<v8::Function> handler = v8::Persistent<v8::Function>::New(cb);

      data->handler = handler;
      data->timeout = repeated?timeout:-1;

      ::roboto::Timer::timed( (long)(timeout * 1000), executeTimedCallback, data );
    }
    
    return v8::Undefined();
  }
  
  /*
  // one-shot timer
  long setTimeout(Function handler, optional long timeout, any... args);
  long setTimeout([AllowAny] DOMString handler, optional long timeout, any... args);
  void clearTimeout(long handle);
  TODO: arguments
  */
  static v8::Handle<v8::Value> v8setTimeout( const v8::Arguments& args ){
    v8::HandleScope handlescope;
    return scheduleTimer( args, false );
  }
  static v8::Handle<v8::Value> v8clearTimeout( const v8::Arguments& args ){
    v8::HandleScope handlescope;
    return v8::Undefined();
  }
  /*
  // repeating timer
  long setInterval(Function handler, optional long timeout, any... args);
  long setInterval([AllowAny] DOMString handler, optional long timeout, any... args);
  void clearInterval(long handle);
  */
  static v8::Handle<v8::Value> v8setInterval( const v8::Arguments& args ){
    v8::HandleScope handlescope;
    scheduleTimer( args, true );
  }
  static v8::Handle<v8::Value> v8clearInterval( const v8::Arguments& args ){
    v8::HandleScope handlescope;
    return v8::Undefined();
  }
  
  /*
  requestanimationframe. Currently only shimmed as setTimeout(fn, 15);
  long requestAnimationFrame(FrameRequestCallback callback);
  void cancelAnimationFrame(long handle);
  */
  static v8::Handle<v8::Value> v8requestAnimationFrame( const v8::Arguments& args ){
    v8::HandleScope handlescope;
    return v8::Undefined();
  }
  static v8::Handle<v8::Value> v8cancelAnimationFrame( const v8::Arguments& args ){
    v8::HandleScope handlescope;
    return v8::Undefined();
  }
  
  // gets called by Window::getTemplate
  Window::Window(v8::Handle<v8::Object>& that) : EventTarget(that)  {
    that->Set( v8::String::New("window"), that );
    that->Set( v8::String::New("document"), Document::getTemplate()->GetFunction()->NewInstance() );
    that->Set( v8::String::New("console"), Console::getTemplate()->GetFunction()->NewInstance() );
    V8_ADD_FUNCTION( that, setTimeout );
    V8_ADD_FUNCTION( that, clearTimeout );
    
    V8_ADD_FUNCTION( that, setInterval );
    V8_ADD_FUNCTION( that, clearInterval );
    
    V8_ADD_FUNCTION( that, requestAnimationFrame );
    V8_ADD_FUNCTION( that, cancelAnimationFrame );
  }

  v8::Handle<v8::ObjectTemplate> Window::New(){
    v8::HandleScope scope;
    
    v8::Handle<v8::ObjectTemplate> tmpl = Window::getTemplate()->InstanceTemplate();
    tmpl->SetInternalFieldCount(1);
    
    return scope.Close( tmpl );
  }
  
  V8_CREATE_CLASS(Window)
}}
