#include "runtime.h"
#include "timer.h"

// js headers
#include "../js/window.h"


#include <stdlib.h>
#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "robotoJS", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "robotoJS", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "robotoJS", __VA_ARGS__))



/**
 * 
 * The runtime itself
 * 
 */
namespace roboto{
  
  void dummy10(void* d){ 
    LOGI( "Wait dummy. Time: 10 " );
  }
  void dummy0(void* d){ 
    LOGI( "Wait dummy. Time: 0 " );
  }
  void dummy1(void* d){ 
    LOGI( "Wait dummy. Time: 1 " );
  }
  void dummy5(void* d){ 
    LOGI( "Wait dummy. Time: 5" );
  }
  
  /**
   * Initialize the runtime. 
   * This includes initializing v8, creating Scope and Context for the JS executions.
   * @param state the android app state that is passed into the native activity
   */
  Runtime::Runtime(struct android_app* state){
    this->started = false;
    this->state = state;
    this->state->userData = this;

    Timer::initialize(state);
    LOGI("INIT TIMER!!!!!");
    Timer::timed( 10 * 1000 * 1000, &dummy10, NULL );
    Timer::timed( 1 * 1000 * 1000, &dummy1, NULL );
    Timer::timed( 0 * 1000 * 1000, &dummy0, NULL );
    Timer::timed( 5 * 1000 * 1000, &dummy5, NULL );
    
    //Event::initialize(state);
    //Rendering::initialize();
    

    v8::V8::Initialize();
    v8::HandleScope handle_scope;
    // Create a context with the correct global object
    v8::Handle<v8::ObjectTemplate> global = this->createGlobal(); 
    // create Persistent Handle for a context
    this->context = v8::Context::New(NULL, global);
  }
  
  /**<
   * Destructor disposes context object.
   */
  Runtime::~Runtime(){
    this->context.Dispose();
  }
  
  void Runtime::fireEvent(const char* type, AInputEvent* event){
    if( this->started ){
      LOGI(type);
    }
  } 
  
  v8::Handle<v8::ObjectTemplate> Runtime::createGlobal(){
    v8::HandleScope scope;
    
    v8::Handle<v8::ObjectTemplate> global_template = v8::ObjectTemplate::New();
    
    ::roboto::js::Window::initialize( global_template );
    
    return scope.Close(global_template);
  }
  
  /**
   * Starts the event loop. Runs in circles and dispatches events that have come in.
   */
  void Runtime::startEventLoop(){
    
    
    
    while(true) Timer::update();
    //Event::update();
    //Rendering::update();
  }
  
  // public methods
  /**
   * Start up V8 and load the default file (js/main.js)
   */
  void Runtime::start(){
    this->start("js/main.js");
  }

  /**
   * Start up V8 and load a javascript file.
   * If the runtime has already been started, this call will behave like a call to run()
   */
  void Runtime::start(const char* mainFile){
    this->run(mainFile);

    if( !this->started ){
      this->started = true;
      this->startEventLoop();
    }
  }
  
  /**
   * Run a JS file from the assets folder.
   */
  void Runtime::run(const char* name){
    LOGI(name);
    // Create a stack-allocated handle scope.
    v8::HandleScope handle_scope;
    // enter a context for compiling and executing JS
    v8::Context::Scope context_scope(this->context);
    
    // Create a string containing the JavaScript source code.
    v8::Handle<v8::String> source = this->readFile(name);

    v8::TryCatch trycatch;

    // Compile the source code.
    v8::Handle<v8::Script> script = v8::Script::Compile(source);
    
    // Run the script to get the result.
    v8::Handle<v8::Value> result = script->Run();
    
    
    if( result.IsEmpty() ){
      // error?
      v8::Handle<v8::Value> exception = trycatch.Exception();
      v8::String::AsciiValue exception_str(exception);
      LOGE(*exception_str);
    }else{
      // success, convert the result to an ASCII string and print it.
      v8::String::AsciiValue ascii(result);
      LOGI(*ascii);
    }
  }


  
  v8::Handle<v8::String> Runtime::readFile(const char* name){
    AAssetManager* mgr = this->state->activity->assetManager;
    AAsset* asset = AAssetManager_open(mgr, name, AASSET_MODE_UNKNOWN);
    if (NULL == asset) {
      LOGW("Assets not found!");
      //__android_log_print(ANDROID_LOG_ERROR, NF_LOG_TAG, "_ASSET_NOT_FOUND_");
      return v8::String::New("", 0);
    }
    long size = AAsset_getLength(asset);
    char* buffer = new char[size];
    AAsset_read (asset,buffer,size);

    v8::Handle<v8::String> result = v8::String::New(buffer, size);
    
    delete[] buffer;
    buffer = 0;
    
    return result;
  }

} 
