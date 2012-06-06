#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <sys/cdefs.h>


#include <android_native_app_glue.h>

#include "roboto/runtime.h"


/**
 * This is the main entry point of a native application that is using
 * android_native_app_glue.  It runs in its own thread, with its own
 * event loop for receiving input events and doing other things.
 */
__BEGIN_DECLS


void android_main(struct android_app* state) {
  // Make sure glue isn't stripped.
  app_dummy();

  roboto::Runtime runtime(state);
  runtime.start();
  
}

__END_DECLS
