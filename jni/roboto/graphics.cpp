#include "graphics.h"
#include "log.h"
#include "timer.h"
#include "./util/matrix.h"

#include <android_native_app_glue.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <EGL/egl.h>



namespace roboto{
  // TODO: transform coordinates
  static const char vertexShader[] = 
    "uniform mat4 transform;"
    "attribute vec4 position;\n"
    "attribute vec4 inputTextureCoordinate;\n"
    "varying vec4 v_Color;\n"
    "varying vec2 textureCoordinate;\n"
    "void main() {\n"
    "    gl_Position =  transform * position;\n"
    "    textureCoordinate = inputTextureCoordinate.xy;\n"
    "}\n";
  // ouput white for every fragment
  static const char textureFragmentShader[] = 
      "varying highp vec2 textureCoordinate;\n"
      "uniform sampler2D u_Texture;\n"
      "void main() {\n"
      "    gl_FragColor = texture2D(u_Texture, textureCoordinate);\n"
      "}\n";
  static const char colorFragmentShader[] = 
      "void main() {\n"
      "    gl_FragColor = vec4( 1.0, 1.0, 1.0, 1.0 );\n"
      "}\n";
      
  static const GLfloat squareVertices[] = {
    -1.0f, -1.0f,
    1.0f, -1.0f,
    -1.0f,  1.0f,
    1.0f,  1.0f,
  };

  static const GLfloat textureVertices[] = {
    1.0f, 1.0f,
    1.0f, 0.0f,
    0.0f,  1.0f,
    0.0f,  0.0f,
  };
  
  
  void callFrame(void* d){
    static float an = 0.1;
    Graphics* g = (Graphics*)d;
    g->frame();
    //g->scale( 0.9999, 0.9999 );
    //g->rotate( an * 3.141 / 180.0 );
    //g->translate( 0, 0.01 );
   // g->translate( 0.001, 0.001 );
    // update every 10ms (100fps)
    Timer::timed( 10000 ,callFrame, d );
  }

  Graphics::Graphics(android_app* app){
    this->app = app;
    this->changed = true;
    this->initialized = false;

    this->resetTransform();

    callFrame(this);
  }
  
  Graphics::~Graphics(){
    
  }
  
  /*
   * Transformations
   */
  /**
   * Reset transformation matrix to Identity matrix
   */
  void Graphics::resetTransform(){
    this->matrix.loadIdentity();
    this->translate( -1, +1 );
    this->scale( 2.0 / this->width, -2.0 / this->height );
  }
  
  void Graphics::scale(float sx, float sy) {
    this->matrix.scale(sx, sy, 1);
  }
  
  

  void Graphics::translate(float tx, float ty) {
    this->matrix.translate(tx, ty, 0);
  }
 
  void Graphics::rotate(float angle) {
    this->matrix.rotate(angle, 0, 0, 1);
  }
  
  
  
  
  
  
  
  /*
   * Renderbuffer stuff
   */
  
  void Graphics::toRenderBuffer(bool back){
    if( back ){
      
    }else{
      
    }
  }
  
  void Graphics::setDimension(int w, int h){
    this->width = w;
    this->height = h;
    
    this->resetTransform();  
  }
  
  
  void Graphics::fillRect(){
    if( this->initialized ){
      this->toRenderBuffer(true);
      
      // todo: draw vertices
    }
  }
  
  
    
  GLfloat gTriangleVertices[] = { 
        400.0f, 470.0f,
        10.0f, 10.0f,
        790.0f, 10.0f 
  };
    
  void Graphics::frame(){
    if( this->initialized ){
      this->toRenderBuffer(false);
      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


      // TODO: draw renderbuffer texture
      glUseProgram(this->colorProgram);
      this->checkGlError("[frame] glUseProgram");
    
      //GLuint textureCoord = glGetAttribLocation(this->colorProgram, "inputTextureCoordinate"); 
      //this->checkGlError("glGetAttribLocation(inputTexture)");
      GLuint position = glGetAttribLocation(this->colorProgram, "position"); 
      this->checkGlError("glGetAttribLocation(position)");
      GLuint transform =  glGetUniformLocation(this->colorProgram, "transform"); 
      this->checkGlError("glGetAttribLocation(transform)");
      
      
      
      glVertexAttribPointer(position, 2, GL_FLOAT, GL_FALSE, 0, gTriangleVertices);
      glEnableVertexAttribArray(position);
      this->checkGlError("glEnableVertexAttribArray(position)");
      glUniformMatrix4fv(transform, 1, false, this->matrix.get());
      this->checkGlError("glUniformMatrix4fv(matrix)");
      
      glDrawArrays(GL_TRIANGLES, 0, 3);

      
      eglSwapBuffers(this->display, this->surface);
      this->toRenderBuffer(true);
    }else{
      this->setupGraphics();
    }
  }
  
  GLuint Graphics::loadShader(GLenum shaderType, const char* pSource) {
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

  GLuint Graphics::createProgram(const char* pVertexSource, const char* pFragmentSource) {
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
  
  
  bool Graphics::setupGraphics() {
    if( this->app->window == NULL || this->initialized ){
      return false;
    }
    
    this->createContext(app);
    this->textureProgram = this->createProgram(vertexShader, textureFragmentShader);
    this->colorProgram = this->createProgram(vertexShader, colorFragmentShader);
    if (!this->textureProgram || !this->colorProgram) {
        LOGE("Could not create program.");
        return false;
    }
    this->initialized = true;
    return true;
  }
  
  void Graphics::createContext(android_app* app){
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
      EGL_NONE
    };
    
    const EGLint attrib_list [] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
    
    EGLint w, h, dummy, format;
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

    ANativeWindow_setBuffersGeometry(app->window, 0, 0, format);

    surface = eglCreateWindowSurface(display, config, app->window, NULL);
    context = eglCreateContext(display, config, NULL, attrib_list);

    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
      LOGW("Unable to eglMakeCurrent");
    }


    eglQuerySurface(display, surface, EGL_WIDTH, &w);
    eglQuerySurface(display, surface, EGL_HEIGHT, &h);
    this->checkGlError("eglQuerySurface");

    // Initialize GL state.
    //glEnable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    
    glViewport(0, 0, w, h);
    
    this->display = display;
    this->surface = surface;
    
    this->setDimension(w, h);
  }
  
  
  void Graphics::checkGlError(const char* op) {
    int errorCount = 0;
    for (GLint error = glGetError(); error; error
            = glGetError()) {
        LOGI("after %s() glError (0x%x)\n", op, error);
        errorCount++;
    }
    this->app->destroyRequested = errorCount;
  }
  
}
