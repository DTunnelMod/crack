#include <jni.h>
#include <pthread.h>
#include <map>
#include "./includes/Crack.h"

int RegisterMethods(JNIEnv *env) {

    JNINativeMethod methods[] = {
        {"config", "(IZLjava/lang/String;)Ljava/lang/String;", reinterpret_cast<void *>(config)},
    };
    
    jclass clazz = env->FindClass("com/kirito/cracker/DTunnel");

    if (!clazz)
        return JNI_ERR;

    if (env->RegisterNatives(clazz, methods, sizeof(methods) / sizeof(methods[0])) != 0)
        return JNI_ERR;

    return JNI_OK;
}

JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved) {

    JNIEnv *env;
    vm->GetEnv((void **)&env, JNI_VERSION_1_6);

    if (RegisterMethods(env) != 0) return JNI_ERR;

    return JNI_VERSION_1_6;
}

JNIEXPORT void JNICALL
JNI_OnUnload(JavaVM *vm, void *reserved) {}