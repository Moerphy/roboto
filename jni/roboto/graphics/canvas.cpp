#include "canvas.h"
#include "log.h"
#include "timer.h"
#include "matrix.h"
#include "texture.h"

#include <android_native_app_glue.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <EGL/egl.h>

/*

 */

namespace roboto{
  
  GLuint Canvas::colorProgram = 0;
  GLuint Canvas::textureProgram = 0;
 
  bool Canvas::initialized = false;
 
  Canvas::Canvas(){
    this->changed = false;
    this->renderBuffer = 0;
    this->renderTexture = 0;
    this->bufferReady = false;
  }

  Canvas::~Canvas(){}
  
  bool Canvas::createRenderBuffer(){
    // don't initialized buffer since either openGL context is not set up yet or buffer is already created
    if( !Canvas::initialized || this->renderBuffer > 0 ){
      this->bufferReady = (this->renderBuffer>0);
      return false;
    }
    //LOGI("Creating renderbuffer..");
    
    int w = this->width;
    int h = this->height;
    
    //LOGI("Width: %i, Height: %i", w, h );
    
    glGenTextures(1, &(this->renderTexture));
    glBindTexture(GL_TEXTURE_2D, this->renderTexture);
    this->checkGlError("setupRenderBuffer.glBindTexture");
    
    // configure texture
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap
    this->checkGlError("setupRenderBuffer.glTexParameter");
    // generate texture image
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 
                 w, h, 0, GL_RGBA, 
                 GL_UNSIGNED_BYTE, NULL);
    this->checkGlError("setupRenderBuffer.glTexImage2d");
    //glGenerateMipmap(GL_TEXTURE_2D);
    this->checkGlError("setupRenderBuffer.glGenerateMipmap");
    glBindTexture(GL_TEXTURE_2D, 0);
    
    // create a renderbuffer object to store depth info TODO
    /*
    GLuint rboId;
    glGenRenderbuffers(1, &rboId);
    glBindRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
    //*/
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    
    // create framebuffer object
    glGenFramebuffers( 1, &(this->renderBuffer) );
    glBindFramebuffer( GL_FRAMEBUFFER, this->renderBuffer );
    
    // attach the texture to framebuffer color attachment point
    glFramebufferTexture2D( 
          GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
          GL_TEXTURE_2D, this->renderTexture, 0 );
          
    // attach the renderbuffer to depth attachment point
    //glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboId );
    
    GLuint status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
      LOGE("Canvas.createRenderBuffer. Framebuffer not complete!");
    }
    this->toRenderBuffer(true);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
    
    this->textureImage.id = this->renderTexture;
    this->textureImage.width = this->width;
    this->textureImage.height = this->height;
    
    this->bufferReady = true;
    
    return true;
  }

  void Canvas::toRenderBuffer(bool toBuffer){
    // TODO: do I need to finish all operations manually here?
    //glFlush();
    //glFinish();
    if( toBuffer ){
      // create renderbuffer on first usage
      if( !this->bufferReady ){
        this->createRenderBuffer();
      }
      glBindFramebuffer(GL_FRAMEBUFFER, this->renderBuffer);
    }else{
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    //glViewport(0, 0, this->width, this->height);
  }
  
  bool Canvas::hasChanged(){
    return this->changed; // TODO
  }

  // standard methods of the HTML canvas
  // drawImage(image, x, y)
  void Canvas::drawImage(Texture* i, float dx, float dy ){
    this->drawImage(i, 0, 0, i->width, i->height, dx, dy, i->width, i->height );
  }
  // drawImage(image, sx, sy, sWidth, sHeight, dx, dy, dWidth, dHeight)
  void Canvas::drawImage(Texture* tex, float sx, float sy, float sw, float sh, float dx, float dy, float dw, float dh ){
    this->toRenderBuffer(true);
    glUseProgram(Canvas::textureProgram);
    this->checkGlError("glUseProgram(textureProgram)");
    // fullscreen vertex
    GLfloat vertices[] = {
      dx,     dy+dh, //top left corner
      dx+dw,  dy+dh, //top right corner
      dx,     dy, //bottom left corner
      dx+dw,  dy
    }; // bottom right corner

    GLushort indices[] = { 0, 1, 2, 3 }; //{ 0, 1, 2, 0, 2, 3 };

    GLfloat textureVertices[] = {
      0.0f,  1.0f, // top left
      1.0f, 1.0f, // top right
      0.0f,  0.0f, // bottom left
      1.0f, 0.0f // top left
    };
    
    // bind texture 
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex->id);
    this->checkGlError("glBindTexture");
    //GLuint textureCoord = glGetAttribLocation(this->colorProgram, "inputTextureCoordinate"); 
    //this->checkGlError("glGetAttribLocation(inputTexture)");
    GLuint position = glGetAttribLocation(Canvas::textureProgram, "position"); 
    this->checkGlError("glGetAttribLocation(position)");
    GLuint transform =  glGetUniformLocation(Canvas::textureProgram, "transform"); 
    this->checkGlError("glGetAttribLocation(transform)");
    
    GLuint textureh =  glGetUniformLocation(Canvas::textureProgram, "u_Texture");
    this->checkGlError("glGetAttribLocation(texture)");
    GLuint texCoordsh = glGetAttribLocation(Canvas::textureProgram, "inputTextureCoordinate"); 
    this->checkGlError("glGetAttribLocation(inputTextureCoordinate)");
    
    glVertexAttribPointer(position, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glEnableVertexAttribArray(position);
    this->checkGlError("glEnableVertexAttribArray(position)");
    glUniformMatrix4fv(transform, 1, false, this->transform.get());
    this->checkGlError("glUniformMatrix4fv(matrix)");
    glVertexAttribPointer(texCoordsh, 2, GL_FLOAT, GL_FALSE, 0, textureVertices);
    glEnableVertexAttribArray(texCoordsh);
    this->checkGlError("glVertexAttribPointer(texCoordsh)");

    // Set the sampler texture unit to 0
    glUniform1i(textureh, 0);
   
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, indices);
    
  }
  
  void Canvas::drawImage(Canvas* i, float sx, float sy, float sw, float sh, float dx, float dy, float dw, float dh ){
    this->drawImage( &(i->getTexture()), sx, sy, sw, sh, dx, dy, dw, dh );
  }
  
  void Canvas::fillRect(){ // TODO: only dummy test method
    this->toRenderBuffer(true);
    GLfloat gTriangleVertices[] = { 
        400.0f, 400.0f,
        10.0f, 10.0f,
        790.0f, 10.0f 
    };
    
    bool shadersOk = (Canvas::colorProgram!=0) && (Canvas::textureProgram!=0);
    glUseProgram(Canvas::colorProgram);
    this->checkGlError("[fillRect] glUseProgram");
  
    //GLuint textureCoord = glGetAttribLocation(this->colorProgram, "inputTextureCoordinate"); 
    //this->checkGlError("glGetAttribLocation(inputTexture)");
    GLuint position = glGetAttribLocation(Canvas::colorProgram, "position"); 
    this->checkGlError("glGetAttribLocation(position)");
    GLuint transform =  glGetUniformLocation(Canvas::colorProgram, "transform"); 
    this->checkGlError("glGetAttribLocation(transform)");

    
    glVertexAttribPointer(position, 2, GL_FLOAT, GL_FALSE, 0, gTriangleVertices);
    glEnableVertexAttribArray(position);
    this->checkGlError("glEnableVertexAttribArray(position)");
    glUniformMatrix4fv(transform, 1, false, this->transform.get());
    this->checkGlError("glUniformMatrix4fv(matrix)");
    
    glDrawArrays(GL_TRIANGLES, 0, 3);
    
    glDisableVertexAttribArray(position);
    this->changed = true;
    LOGI("Called fillRect()");
  }
  
  
  void Canvas::scale(float sx, float sy){
    this->transform.scale(sx, sy, 1);  
  }
  void Canvas::translate(float tx, float ty){
    this->transform.translate(tx, ty, 0);
  }
  void Canvas::rotate(float angle){
    this->transform.rotate(angle, 0, 0, 1);
  }

  
  void Canvas::save(){}
  void Canvas::restore(){}
  
  // non-standard methods
  void Canvas::resetTransform(){
    this->transform.loadIdentity();
    this->translate( -1, +1 );
    this->scale( 2.0 / this->width, -2.0 / this->height );
  }
  
  void Canvas::setDimension(int w, int h){
    this->width = w;
    this->height = h;
    this->resetTransform();
    this->clear();
  }
  
  Texture& Canvas::getTexture(){
    return this->textureImage;
  }
  void Canvas::clear(){
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
  }
  
  void Canvas::checkGlError(const char* op) {
    for (GLint error = glGetError(); error; error = glGetError()) {
        LOGI("after %s() glError (0x%x)\n", op, error);
    }
  }
  
}
