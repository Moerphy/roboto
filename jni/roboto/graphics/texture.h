#ifndef ROBOTO_TEXTURE_H
#define ROBOTO_TEXTURE_H

#include <android_native_app_glue.h>

#include <GLES2/gl2.h>

namespace roboto{
  
  class Texture{
    public:
      GLuint id;
      int width;
      int height;
      const char* filename;
      
    private:
    
    public:
      Texture();
      ~Texture();

      void dispose();

      static Texture* load(const char* filename);
      static void initialize(android_app*);
  };
  
  
  
  //Filename will be looked up in the apk (should start with assets/ or res/
  GLuint loadTextureFromPNG (const char* filename, int &width, int &height);
}
#endif /* UTILS_H_ */
