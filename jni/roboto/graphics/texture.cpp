#include "../log.h"

#include "texture.h"

/*
#include <GLES2/gl2.h>
extern "C" {
#include <png.h>
}
//#include <stdio.h>
#include <zip.h>
*/

namespace roboto{
  
  android_app* app;
  jclass textureLoaderClass;
  
  Texture::Texture(){
    this->id = 0;
    this->width = -1;
    this->height = -1;
    this->filename = "";
  }
  
  Texture::~Texture(){
    // don't delete texture id here, textures should be disposed manually
  }
  
  void Texture::dispose(){
    if( this->width > 0 && this->height > 0 ){
      glDeleteTextures(1, &(this->id));
    }
  }
  
  Texture* Texture::load(const char* filename){
    // TODO
    // generate texture
    Texture* t = new Texture();
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    
    // call java code
    JNIEnv *jni; 
    app->activity->vm->AttachCurrentThread(&jni, NULL); 
    
    jstring filenameString = jni->NewStringUTF(filename);
    jint textureIdJava = textureId;
    
    LOGI("Calling texture.load");
    
    //jclass textureLoaderClass = jni->FindClass("org/metafnord/roboto/TextureLoader");
    jmethodID loadMethod = jni->GetStaticMethodID(textureLoaderClass, "loadTexture", "(ILjava/lang/String;)[I"); // "(ILjava/lang/String;)[I"
    jintArray dimensions = (jintArray)
          jni->CallStaticObjectMethod( textureLoaderClass, loadMethod , textureIdJava, filenameString );
    
    jint buf[2];
    jni->GetIntArrayRegion(dimensions, 0, 2, buf);
    
    int w = buf[0];
    int h = buf[1];

    LOGI("Loaded texture %s; Dimensions: (%i/%i); id: %i", filename, w, h, textureId );

    app->activity->vm->DetachCurrentThread(); 
    
    // put return values in a texture object
    
    t->filename = filename;
    t->id = textureId;
    t->width = w;
    t->height = h;
    
    return t;
  }
  
  void Texture::initialize(android_app* state){
    app = state;

    JNIEnv *jni; 
    app->activity->vm->AttachCurrentThread(&jni, NULL); 
    
    jclass activityClass = jni->FindClass("android/app/NativeActivity");
    jmethodID getClassLoader = jni->GetMethodID(activityClass,"getClassLoader", "()Ljava/lang/ClassLoader;"); 
    jobject cls = jni->CallObjectMethod(app->activity->clazz, getClassLoader); 
    jclass classLoader = jni->FindClass("java/lang/ClassLoader"); 
    jmethodID findClass = jni->GetMethodID(classLoader, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;"); 
    jstring strClassName = jni->NewStringUTF("org/metafnord/roboto/TextureLoader");
    textureLoaderClass = (jclass)jni->CallObjectMethod(cls, findClass, strClassName);
    
    app->activity->vm->DetachCurrentThread(); 
  }

}

/*
 
JNIEnv *jni; 
state->vm->AttachCurrentThread(&jni, NULL); 
jclass activityClass = jni->FindClass("android/app/NativeActivity");
jmethodID getClassLoader = jni->GetMethodID(activityClass,"getClassLoader", "()Ljava/lang/ClassLoader;"); 
jobject cls = jni->CallObjectMethod(state->activity->clazz, getClassLoader); 
jclass classLoader = jni->FindClass("java/lang/ClassLoader"); 
jmethodID findClass = jni->GetMethodID(classLoader, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;"); 
jstring strClassName = jni->NewStringUTF("com/tewdew/ClassIWant");
jclass classIWant = (jclass)jni->CallObjectMethod(cls, findClass, strClassName);
 
 *
 JNIEXPORT jint JNICALL Java_Client_getAgeC(JNIEnv *env, jobject callingObject, jobject employeeObject) {
    jclass employeeClass = (*env)->GetObjectClass(env, employeeObject);
    jmethodID midGetName = (*env)->GetMethodID(env, employeeClass, "getAge", "()I");
    int age =  (*env)->CallIntMethod(env, employeeObject, midGetName);
    return age;
}
 * 
 // pass value from c to java
JNIEXPORT jobject JNICALL Java_Client_createWithAge(JNIEnv *env, jobject callingObject, jint age) {
    jclass employeeClass = (*env)->FindClass(env,"LEmployee;");
    jmethodID midConstructor = (*env)->GetMethodID(env, employeeClass, "<init>", "(I)V");
    jobject employeeObject = (*env)->NewObject(env, employeeClass, midConstructor, age);
    return employeeObject;
}
* 
jmethodID functionID = env->GetMethodID(
    activityClass,
    "CallBack", 
    "(Ljava/lang/String;)V");
env->CallVoidMethod(
    obj,
    functionID,
    env->NewStringUTF(argToPass));
*/
