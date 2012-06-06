#ifndef ROBOTO_RUNTIME_H
#define ROBOTO_RUNTIME_H

#include <android_native_app_glue.h>
#include <v8.h>


namespace roboto{

  class Runtime{
    private:
      // properties
      bool started;
      struct android_app* state;
      
      
      // methods
      // add extensions to V8 (methods and functions that should be on the global object)
      v8::Persistent<v8::Context> createContext();
      
      void startEventLoop();

      // Reads a file into a v8 string.
      v8::Handle<v8::String> readFile(const char* name);
      
    public:
      v8::Persistent<v8::Context> context;
    
      Runtime(struct android_app* state);
      ~Runtime();
      void start();
      void start(const char* main);
      void run(const char* name);
      
      void fireEvent(const char* type, AInputEvent* event);
  };

}

#endif
