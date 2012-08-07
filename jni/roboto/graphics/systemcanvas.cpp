#include "systemcanvas.h"
#include "canvas.h"
#include "log.h"
#include "timer.h"
#include "matrix.h"


#include <android_native_app_glue.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <EGL/egl.h>

/*

 */

namespace roboto{
  // Canvas shaders
  static const char vertexShader[] = 
    "uniform mat4 transform;"
    "attribute vec4 position;\n"
    "attribute vec2 inputTextureCoordinate;\n"
    "varying vec4 v_Color;\n"
    "varying vec2 textureCoordinate;\n"
    "void main() {\n"
    "    gl_Position =  transform * position;\n"
    "    textureCoordinate = inputTextureCoordinate;\n"
    "}\n";
  static const char textureFragmentShader[] = 
      "varying highp vec2 textureCoordinate;\n"
      "uniform sampler2D u_Texture;\n"
      "void main() {\n"
      "    gl_FragColor = vec4( 1.0, 1.0, 1.0, 1.0 ) * texture2D(u_Texture, textureCoordinate);\n"
      "}\n";
  // TODO: colors
  static const char colorFragmentShader[] = 
      "void main() {\n"
      "    gl_FragColor = vec4( 1.0, 1.0, 1.0, 1.0 );\n"
      "}\n";

  SystemCanvas* instance = NULL;
  
  /**
   * Refresh onscreen canvas
   * TODO don't update in intervals, but on request of the android framework
   */
  void updateFrame(void* d){
    if( d != NULL ){
      SystemCanvas* g = static_cast<SystemCanvas*>(d);
      g->frame();
      // update every 16ms (60fps)
      Timer::timed( 16000 ,updateFrame, d );
    }
  }
  
  // singleton constructor, therefore only called once
  SystemCanvas::SystemCanvas() : Canvas(){
    this->window = NULL;
    this->canvasList = NULL;
    this->renderBuffer = 0;
    
    this->bufferReady = true;

    updateFrame(this);
  }
  
  SystemCanvas::~SystemCanvas(){
    
  }
  
  void SystemCanvas::initialize(ANativeWindow* win){
    if( !Canvas::initialized ){
      this->window = win;
      
      Canvas::initialized = this->setupGraphics();
    }
    
    Canvas* bc = this->createCanvas();
    this->addCanvas(bc);
    Canvas* bc2 = this->createCanvas();
    this->addCanvas(bc2);
    bc->scale(5, 5);
    bc2->scale(5, 5);
    //bc->scale( 10, 10 );
    Texture* t = Texture::load("img/sprite.png");
    bc->drawImage(t, 0, 0);
    bc2->drawImage(t, 32, 32);
    
    t->dispose();
    delete t;
    /*
    bc->translate( 100, 100 );
    bc->fillRect();
    bc->translate( 100, 100);
    bc->fillRect();
    //*/
  }


  void SystemCanvas::frame(){
    if( Canvas::initialized && this->hasChanged() ){
      this->drawFrame();
    }
  }
  
  /**
   * Returns true if one of the canvases has changed since the last frame
   */
  bool SystemCanvas::hasChanged(){
    CanvasItem* list = this->canvasList;
    while( list != NULL ){
      if( list->canvas->hasChanged() ){
        return true;
      }
      list = list->next;
    }
    return false;
  }
  
  void SystemCanvas::drawFrame(){
    // don't render to backbuffer, but to the screen
    this->toRenderBuffer(false);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
    // draw all frames, check z-Indexes to reorder them if necessary (TODO)
    int texCount = 0;
    CanvasItem* list = this->canvasList;
    while( list != NULL ){
      Texture t = list->canvas->getTexture();
      list->canvas->setChanged(false);
      this->drawImage(&t, 0, 0); // TODO
      list = list->next;
      texCount++;
    }
    LOGI("SystemCanvas.drawFrame. %i", texCount);
    // swap buffers
    eglSwapBuffers(this->display, this->surface);
    // clear back buffer for next frame
    
    // reset rendering mode
    
  }

  
  bool SystemCanvas::setupGraphics(){
    if( this->window == NULL || Canvas::initialized ){
      LOGI("Returning early: %s / %s", (this->window==NULL)?"NULL":"Window", Canvas::initialized?"true":"false" );
      return false;
    }
   
    bool contextCreated = this->createContext();
    bool shadersCreated = this->createShaders();
    //this->createRenderBuffer();
    
    // clean Buffers
    eglSwapBuffers(this->display, this->surface);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
    eglSwapBuffers(this->display, this->surface);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

    if( !contextCreated || !shadersCreated ){
      LOGE("SystemCanvas.setupGraphics. Could not set up. Context: %i, Shaders: %i", contextCreated, shadersCreated);
    }
    
    return contextCreated && shadersCreated;
  }
  
  bool SystemCanvas::createContext(){
    // initialize OpenGL ES and EGL

    /*
     * Here specify the attributes of the desired configuration.
     * Below, we select an EGLConfig with at least 8 bits per color
     * component compatible with on-screen windows
     */
    const EGLint attribs[] = {
      EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT, //important 
      EGL_BLUE_SIZE, 8,
      EGL_GREEN_SIZE, 8,
      EGL_RED_SIZE, 8,
      //EGL_SURFACE_TYPE, EGL_SWAP_BEHAVIOR_PRESERVED_BIT,
      EGL_NONE
    };
    
    const EGLint attrib_list [] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
    
    EGLint w, h, format;
    EGLint numConfigs;
    EGLConfig config;
    EGLSurface surface;
    EGLContext context;

    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    eglInitialize(display, 0, 0);

    /* Here, the application chooses the configuration it desires. In this
     * sample, we have a very simplified selection process, where we pick
     * the first EGLConfig that matches our criteria */
    eglChooseConfig(display, attribs, &config, 1, &numConfigs);

    /* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
     * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
     * As soon as we picked a EGLConfig, we can safely reconfigure the
     * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

    ANativeWindow_setBuffersGeometry(this->window, 0, 0, format);

    surface = eglCreateWindowSurface(display, config, this->window, NULL);
    context = eglCreateContext(display, config, NULL, attrib_list);

    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
      LOGW("Unable to eglMakeCurrent");
      return false;
    }

    //eglSurfaceAttrib( display, surface, EGL_SWAP_BEHAVIOR, EGL_BUFFER_PRESERVED);

    eglQuerySurface(display, surface, EGL_WIDTH, &w);
    eglQuerySurface(display, surface, EGL_HEIGHT, &h);
    this->checkGlError("eglQuerySurface");

    // Initialize GL state.
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    
    glViewport(0, 0, w, h);
    
    this->display = display;
    this->surface = surface;
    
    LOGI("Systemcanvas.createContext. Width: %i, Height: %i", w, h);
    
    this->setDimension((int)w, (int)h);
    
    return true;
  }
  

  
  bool SystemCanvas::createShaders(){
    Canvas::textureProgram = this->createShaderProgram(vertexShader, textureFragmentShader);
    Canvas::colorProgram = this->createShaderProgram(vertexShader, colorFragmentShader);
    if ( Canvas::textureProgram == 0 || Canvas::colorProgram == 0 ) {
        LOGE("Could not create program. %i / %i", Canvas::textureProgram, Canvas::colorProgram);
        return false;
    }
    return true;
  }
  
  GLuint SystemCanvas::createShaderProgram(const char* pVertexSource, const char* pFragmentSource){
    GLuint vertexShader = this->loadShader(GL_VERTEX_SHADER, pVertexSource);
      if (!vertexShader) {
          LOGE("Could not create vertexshader");
          return 0;
      }

      GLuint pixelShader = this->loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
      if (!pixelShader) {
          LOGE("Could not create fragmentshader");
          return 0;
      }

      GLuint program = glCreateProgram();
      if (program) {
          glAttachShader(program, vertexShader);
          this->checkGlError("glAttachShader");
          glAttachShader(program, pixelShader);
          this->checkGlError("glAttachShader");
          glLinkProgram(program);
          
          GLint linkStatus = GL_FALSE;
          glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
          if (linkStatus != GL_TRUE) {
              GLint bufLength = 0;
              glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
              if (bufLength) {
                  char* buf = (char*) malloc(bufLength);
                  if (buf) {
                      glGetProgramInfoLog(program, bufLength, NULL, buf);
                      LOGE("Could not link program:\n%s\n", buf);
                      free(buf);
                  }
              }
              glDeleteProgram(program);
              program = 0;
          }
      }
      return program;
  }
  
  GLuint SystemCanvas::loadShader(GLenum shaderType, const char* pSource) {
      GLuint shader = glCreateShader(shaderType);
      
      this->checkGlError("glCreateShader");
      
      if (shader != 0) {
          glShaderSource(shader, 1, &pSource, NULL);
          this->checkGlError("glShaderSource");
          glCompileShader(shader);
          this->checkGlError("glCompileShader");
          GLint compiled = 0;
          glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
          this->checkGlError("glGetShaderiv");
          if ( compiled == 0 ) {
              GLint infoLen = 0;
              glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
              this->checkGlError("glShaderiv (infolen)");
              if (infoLen) {
                  char* buf = (char*) malloc(infoLen);
                  if (buf) {
                      glGetShaderInfoLog(shader, infoLen, NULL, buf);
                      LOGE("Could not compile shader %d:\n%s\n",
                              shaderType, buf);
                      free(buf);
                  }
                  glDeleteShader(shader);
                  shader = 0;
              }
          }
      }
      return shader;
  }
  
  
  Canvas* SystemCanvas::createCanvas(){
    Canvas* c = new Canvas();
    
    c->setDimension( this->width, this->height );
    
    return c;
  }

  void SystemCanvas::destroyCanvas(Canvas* c){
    this->removeCanvas(c);
    delete c;
  }

  void SystemCanvas::addCanvas(Canvas* c){
    this->removeCanvas(c);
    CanvasItem* li = new CanvasItem;
    li->canvas = c;
    li->next = this->canvasList;
    
    this->canvasList = li;
  }

  void SystemCanvas::removeCanvas(Canvas* c){} // TODO
  
  // singleton method
  SystemCanvas* SystemCanvas::getInstance(){
    if( instance == NULL ){
      instance = new SystemCanvas();
    }
    return instance;
  }
  //*
  void SystemCanvas::resetTransform(){
    this->transform.loadIdentity();
    this->translate( -1, -1 );
    this->scale( 2.0 / this->width, 2.0 / this->height );
  }
  //*/
}
