#include <jni.h>
#include <string>
//3. 包含日志头文件, 定义日志输出函数
#include <pthread.h>
#include <android/log.h>
#include <sys/select.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unordered_map>
#include "Utils.h"

#define LOG_TAG "atlinweimao"

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

//
void *thread_func(void *arg);

struct MyStruct {
    int a;
    int b;
    int c;
};
typedef struct a {
    int a = 1;
    char b = 'b';
} A;
A as[3];
// 要保护的共享资源
int counter = 0;
int global_var = 0; // 定义全局变量

// 定义互斥锁
pthread_mutex_t mutex;

struct MyStruct ss = {20, 30, 40};
extern "C" JNIEXPORT jstring JNICALL
Java_com_example_jnistudy_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";

    std::string name;
    return env->NewStringUTF(hello.c_str());
}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_jnistudy_MainActivity_adf(JNIEnv *env, jobject thiz, jint a, jboolean b) {

}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_jnistudy_MainActivity_00024Companion_bbb(JNIEnv *env, jobject thiz) {

}

struct s1 {
    char c1;
    int a;
    char c2;
};

struct s2 {
    char c1;
    char c2;
    int a;
};

struct s3 {
    int d;
    char c;
    int i;
    struct s2 t;
};

extern "C"
JNIEXPORT void JNICALL
Java_com_example_jnistudy_MainActivity_updateNameField(JNIEnv *env, jobject thiz) {

    struct s1 structs1 = {0};
    struct s2 structs2 = {0};
    LOGD("[zhang] = sizeof(structs1), %d", sizeof(structs1));
    LOGD("[zhang] = sizeof(structs2), %d", sizeof(structs2));
    LOGD("[zhang] = sizeof(structs3), %d", sizeof(struct s3));

}





//jint JNI_OnLoad(JavaVM *vm, void *reserved) {
////    LOGD("@@@@@@@@@@!!!!!");
//
//}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_jnistudy_MainActivity_writeStudent(JNIEnv *env, jobject thiz, jobject studentObj) {
    jclass studentClass = env->GetObjectClass(studentObj);
    jfieldID nameField = env->GetFieldID(studentClass, "name", "Ljava/lang/String;");
    jstring newName = env->NewStringUTF("Jane");
    env->SetObjectField(studentObj, nameField, newName);
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_example_jnistudy_MainActivity_www(JNIEnv *env,
                                           jobject thiz,
                                           jobject on_status_listener) {
    Utils::setStudentName("hello WOrld");
    std::unordered_map<std::string, int> a =  Utils::getHashMap();
    return nullptr;
}