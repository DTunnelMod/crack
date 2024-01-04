LOCAL_PATH := $(call my-dir)
MAIN_LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE    := Light

# Code optimization
LOCAL_ARM_MODE := arm
LOCAL_CFLAGS := -Wno-error=format-security -fpermissive -fvisibility=hidden -fvisibility-inlines-hidden
LOCAL_CFLAGS += -fno-rtti -g0 -fomit-frame-pointer -ffunction-sections -fdata-sections
LOCAL_CPPFLAGS += -fvisibility=hidden -fexceptions -ffunction-sections -fdata-sections
LOCAL_LDFLAGS += -Wl,--strip-all

LOCAL_STATIC_LIBRARIES := curl_static

# Here you add the cpp file
LOCAL_C_INCLUDES += $(MAIN_LOCAL_PATH)
LOCAL_SRC_FILES := src/main.cpp \
	src/http/cpr/async.cpp \
	src/http/cpr/auth.cpp \
	src/http/cpr/bearer.cpp \
	src/http/cpr/cookies.cpp \
	src/http/cpr/cprtypes.cpp \
	src/http/cpr/curl_container.cpp \
	src/http/cpr/curlholder.cpp \
	src/http/cpr/error.cpp \
	src/http/cpr/multipart.cpp \
	src/http/cpr/parameters.cpp \
	src/http/cpr/payload.cpp \
	src/http/cpr/proxies.cpp \
	src/http/cpr/proxyauth.cpp \
	src/http/cpr/session.cpp \
	src/http/cpr/threadpool.cpp \
	src/http/cpr/timeout.cpp \
	src/http/cpr/unix_socket.cpp \
	src/http/cpr/util.cpp \
	src/http/cpr/response.cpp \
	src/http/cpr/redirect.cpp \
	src/http/cpr/interceptor.cpp \

LOCAL_LDLIBS := -llog -landroid
include $(BUILD_SHARED_LIBRARY)

ifneq ($(call ndk-major-at-least,21),true)
    $(call import-add-path,$(NDK_GRADLE_INJECTED_IMPORT_PATH))
endif

$(call import-module,prefab/curl)