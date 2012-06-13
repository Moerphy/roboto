LOCAL_PATH:= $(call my-dir)



include $(CLEAR_VARS)

LOCAL_MODULE            := v8_base
LOCAL_SRC_FILES         := ./v8/libv8_base.a

include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE            := v8_snap
LOCAL_SRC_FILES         := ./v8/libv8_snapshot.a

include $(PREBUILT_STATIC_LIBRARY)



include $(CLEAR_VARS)

LOCAL_MODULE    := roboto

LOCAL_CFLAGS := -g

LOCAL_SRC_FILES:= js/console.cpp \
	js/window.cpp \
	roboto/eventhandler.cpp \
  roboto/eventsource.cpp \
  roboto/timer.cpp \
  roboto/util/matrix.cpp \
  roboto/graphics.cpp \
  roboto/runtime.cpp \
  roboto.cpp


LOCAL_C_INCLUDES := \
		$(LOCAL_PATH)/roboto/. \
    $(LOCAL_PATH)/. \
    $(LOCAL_PATH)/v8/include/. \
    $(LOCAL_PATH)/li
    
LOCAL_LDLIBS    := -lz -llog -landroid -lEGL  -lGLESv2

LOCAL_STATIC_LIBRARIES := android_native_app_glue v8_base v8_snap

APP_ABI := armeabi armeabi-v7a

include $(BUILD_SHARED_LIBRARY) 

$(call import-module,android/native_app_glue)

