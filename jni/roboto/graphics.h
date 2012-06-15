#ifndef ROBOTO_RENDERING_H
#define ROBOTO_RENDERING_H

#include "./util/matrix.h"

#include <android_native_app_glue.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <EGL/egl.h>


namespace roboto{
  
  class Graphics{
    private: // properties
      bool  changed;
      GLuint textureProgram;
      GLuint colorProgram;
      
      GLuint framebuffer;
      GLuint texture;
      
      EGLDisplay display;
      EGLSurface surface;
      
      int width;
      int height;
      
      Matrix3D matrix;
      
      float* path;
      int maxPathLength;
      int pathLength;
      
      int zIndex;
      
    public:
      android_app* app;
      bool initialized;


    private: // methods
      GLuint createProgram(const char* pVertexSource, const char* pFragmentSource);
      GLuint loadShader(GLenum shaderType, const char* pSource);
      void  checkGlError(const char* op); 
      void createContext(android_app* app);
      bool   setupGraphics();
      void setupRenderBuffer();
      void toRenderBuffer(bool);
      void resetTransform();
      
      void transpose();
      
    public: 
      Graphics(android_app*);
      ~Graphics();
      
      void frame();
      
      void fillRect();
      
      void scale(float, float);
      void translate(float, float);
      void rotate(float);
      
      void setDimension(int, int);
      
      static Graphics* getGraphics();
      static Graphics* getGraphics(int z);
  };
  
}

#endif
