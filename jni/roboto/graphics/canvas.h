#ifndef ROBOTO_GRAPHICS_CANVAS_H
#define ROBOTO_GRAPHICS_CANVAS_H

#include "matrix.h"
#include "texture.h"

#include <android_native_app_glue.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <EGL/egl.h>


namespace roboto{
  
  class Canvas{
    // properties
    private: 
      bool changed;
      Texture textureImage;
    protected:
      static bool initialized;
      // backbuffer
      GLuint renderBuffer;
      GLuint renderTexture;
      // shader programs
      static GLuint textureProgram;
      static GLuint colorProgram;
      
      Matrix3D transform;
      
      int width;
      int height;
      
      bool bufferReady;
      
    public:
      
      
    // methods
    private:
    protected: 
      bool createRenderBuffer();
      void checkGlError(const char*);
      
    public: 
      Canvas();
      ~Canvas();

      void initialize();
      void toRenderBuffer(bool);
      
      // standard methods of the HTML canvas
      void drawImage(Texture* tex, float dx, float dy );
      void drawImage(Texture* tex, float sx, float sy, float sw, float sh, float dx, float dy, float dw, float dh );
      void drawImage(Canvas* i, float sx, float sy, float sw, float sh, float dx, float dy, float dw, float dh );
      
      void scale(float, float);
      void translate(float, float);
      void rotate(float);
      
      void save();
      void restore();
      
      // non-standard methods
      virtual void resetTransform();
      void clear();
      bool hasChanged();
      void setDimension(int, int);
      Texture& getTexture();
      
      
      // TODO dummy
      void fillRect();
  };
  
}

#endif
