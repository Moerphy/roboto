#include "console.h"

#include <android/log.h>
#include <v8.h>
// TODO: formatting arguments

namespace roboto {
namespace js {

  static v8::Handle<v8::Value> print( android_LogPriority priority, const v8::Arguments& args ){
    if (args.Length() >= 1){
      v8::Handle<v8::Value> arg = args[0];
      v8::String::AsciiValue ascii(arg);
      
      __android_log_print(priority, "console", *ascii);
    }
    return v8::Undefined();
  }
  
  
  
  static v8::Handle<v8::Value> error( const v8::Arguments& args ){
    return print( ANDROID_LOG_ERROR, args );
  }
  static v8::Handle<v8::Value>info( const v8::Arguments& args ){
    return print( ANDROID_LOG_INFO, args );
  }
  static v8::Handle<v8::Value> log( const v8::Arguments& args ){
    return print( ANDROID_LOG_VERBOSE, args );
  }
  static v8::Handle<v8::Value> warn( const v8::Arguments& args ){
    return print( ANDROID_LOG_WARN, args );
  }
  


  Console::Console() {
    
  }
  
  
  static v8::Handle<v8::Value> v8NewConsole( const v8::Arguments& args ){
    //LOGI("new Console()");
    v8::HandleScope handlescope;
    v8::Handle<v8::Object> that = args.This();
    
    Console* con = new Console();
    
    that->SetInternalField( 0, v8::External::New( con ) );
    
    that->Set( v8::String::New("error"), v8::FunctionTemplate::New( error )->GetFunction() );
    that->Set( v8::String::New("info"), v8::FunctionTemplate::New( info )->GetFunction() );
    that->Set( v8::String::New("log"), v8::FunctionTemplate::New( log )->GetFunction() );
    that->Set( v8::String::New("warn"), v8::FunctionTemplate::New( warn )->GetFunction() );
    
    
    return args.This();
  }
  
  v8::Handle<v8::FunctionTemplate> Console::getTemplate(){
    v8::HandleScope scope;
    
    v8::Handle<v8::FunctionTemplate> tmpl = v8::FunctionTemplate::New(v8NewConsole);
    tmpl->InstanceTemplate()->SetInternalFieldCount(1);
    
    return scope.Close( tmpl );
  }

}}
