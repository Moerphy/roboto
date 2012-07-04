#ifndef ROBOTO_GRAPHICS_SYSTEM_CANVAS_H
#define ROBOTO_GRAPHICS_SYSTEM_CANVAS_H

#include "canvas.h"
#include "matrix.h"

#include <android_native_app_glue.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <EGL/egl.h>


namespace roboto{
  
  struct CanvasItem{
    Canvas* canvas;
    CanvasItem* next;
  };
  
  class SystemCanvas : Canvas{
    private: 
      
      ANativeWindow* window;
      EGLDisplay display;
      EGLSurface surface;
    public:
      CanvasItem* canvasList;
    // methods
    private: 
      SystemCanvas();
      
      // used once on initialization
      
      bool setupGraphics();
      bool createContext();
      bool createShaders();
      GLuint createShaderProgram(const char*, const char*);
      GLuint loadShader(GLenum, const char*);
      
      bool hasChanged();
      void drawFrame();
    public: 
      ~SystemCanvas();
      void initialize(ANativeWindow*);
      
      /**
       * Composite all the canvases that were added to the system canvas
       */
      void frame();

      /**
       * Create offscreen canvas object.
       * When the Canvas is added to the systemcanvas via addCanvas(), 
       * the offscreen canvas is rendered to the system canvas on the next frame
       */
      Canvas* createCanvas();
      
      /**
       * Remove Canvas completely, free the memory and texture it uses
       */
      void destroyCanvas(Canvas*);
      
      /**
       * Make an offscreen canvas visible on the display
       */
      void addCanvas(Canvas*);
      /**
       * Detach visible canvas from display, making it esentially offscreeen again.
       */
      void removeCanvas(Canvas*); 
      
      void resetTransform();
      
      static SystemCanvas* getInstance();
  };
  
}

#endif
