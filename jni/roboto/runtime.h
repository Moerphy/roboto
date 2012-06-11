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
      v8::Persistent<v8::Context> context;
      
      // methods
      /**
       * Creates the global object, including sub objects
       */
      v8::Handle<v8::ObjectTemplate> createGlobal();
      /**
       * Starts up the event loop. Blocking method that is called by start()
       */
      void startEventLoop();
      /**
       * Reads a file in a V8 string from the assets folder. Used to load JS files on demand
       */
      v8::Handle<v8::String> readFile(const char* name);
      
    public:
      Runtime(struct android_app* state);
      ~Runtime();
      
      void start();
      void start(const char* main);
      void run(const char* name);
      
      void fireEvent(const char* type, AInputEvent* event);
  };

}

#endif
