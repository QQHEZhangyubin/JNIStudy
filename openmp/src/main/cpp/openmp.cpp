#include <jni.h>
#include <string>
#include <omp.h>
#include <android/log.h>
#include <unistd.h>

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_openmp_NativeLib_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    const int MAXPATH = 256;
    char buffer[MAXPATH];
    getcwd(buffer, MAXPATH);

    __android_log_print(ANDROID_LOG_INFO, "MyApp", "path:%s", buffer);

#pragma omp parallel num_threads(4)
    {
        int tid = omp_get_thread_num();
        __android_log_print(ANDROID_LOG_INFO, "MyApp", "Hello from thread %d", tid);
    }

    return env->NewStringUTF(hello.c_str());
}