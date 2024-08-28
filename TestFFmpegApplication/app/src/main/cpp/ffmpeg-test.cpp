//
// Created by wangkexin6 on 2024/8/27.
//
#include <jni.h>
#include <string>

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_testffmpegapplication_MainActivity_stringFromFFTest(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from test";
    return env->NewStringUTF(hello.c_str());
}