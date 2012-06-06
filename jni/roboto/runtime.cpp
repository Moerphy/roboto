#include "runtime.h"

#include "../js/window.h"


#include <stdlib.h>
#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "robotoJS", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "robotoJS", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "robotoJS", __VA_ARGS__))




// TODO: refactor event loop in own component

/**
 * 
 */
static int32_t globalInputHandler(struct android_app* app, AInputEvent* event){
  LOGI("INPUT EVENT!");
  
  roboto::Runtime* runtime = (roboto::Runtime*)(app->userData);

  if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION){
    runtime->fireEvent( "touchmove", event );
    return 1;
  } else if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_KEY){
    //LOGI("Received key event: %d", AKeyEvent_getKeyCode(event));
    return 1;
  }
  return 0;
}

static void globalCmdHandler(struct android_app* app, int32_t cmd){
  LOGI("CMD EVENT!");
  switch (cmd) {
    case APP_CMD_SAVE_STATE:
      // the OS asked us to save the state of the app
      break;
    case APP_CMD_INIT_WINDOW:
      // get the window ready for showing
      break;
    case APP_CMD_TERM_WINDOW:
      // clean up the window because it is being hidden/closed
      break;
    case APP_CMD_LOST_FOCUS:
      // if the app lost focus, avoid unnecessary processing (like monitoring the accelerometer)
      break;
    case APP_CMD_GAINED_FOCUS:
      // bring back a certain functionality, like monitoring the accelerometer
      break;
  }
}


/**
 * 
 * The runtime itself
 * 
 */
namespace roboto{
  /**
   * Initialize the runtime. 
   * This includes initializing v8, creating Scope and Context for the JS executions.
   * @param state the android app state that is passed into the native activity
   */
  Runtime::Runtime(struct android_app* state){
    this->started = false;
    this->state = state;
    this->state->userData = this;


    v8::V8::Initialize();
    // 
    v8::HandleScope handle_scope;
    // Create a context with the correct global object
    v8::Handle<v8::ObjectTemplate> global = v8::ObjectTemplate::New();
    
    js::Window::initialize(global);

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
  
  
  /**
   * Starts the event loop. Runs in circles and dispatches events that have come in.
   */
  void Runtime::startEventLoop(){
    this->state->onInputEvent = globalInputHandler;
    this->state->onAppCmd = globalCmdHandler;
    
    // dispatch native events
    while(true){
      int ident;
      //int fdesc;
      int events;
      struct android_poll_source* source;
      
      
      while( (ident = ALooper_pollAll(0, NULL, &events, (void**)&source)) >= 0 ) {
        // process this event
        if( source != NULL ){
          source->process(this->state, source);
        }
      }
    }
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
    
    LOGI(name);
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
