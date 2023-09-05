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
#include <iostream>
#include "Utils.h"
#include "jsoncpp/value.h"
#include "jsoncpp/reader.h"
#include "jsoncpp/json.h"

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
    ////////
    char str[] = "ab12378jjj\n";
    int num = atoi(&str[4]);
    LOGD(" num  = %d", num);

//    Student s = Utils::getStudent();
//    std::string jj = s.getName();
//
//    LOGD(" jj  = %s", jj.c_str());

    Student *s2 = Utils::getStudent2();
    LOGD(" jj  = %s", s2->getName().c_str());
    delete s2;


    Json::Value video;
    video["id"] = 1;
    video["name"] = "bekko.zhang@anker-in.com";
    video["name2"] = "张玉彬";
    const char *json_str = video.toStyledString().c_str();
    LOGD("json_str = %s", json_str);
    jstring result = env->NewStringUTF(json_str);

    /////////////
//    在使用jsonCpp写入中文时，如果直接将中文字符串写入json对象中，可能会出现汉字内码的情况。
//    这是因为jsonCpp默认使用的是ASCII编码，而中文字符需要使用UTF-8编码才能正确显示。
//    解决这个问题的方法是，在写入json对象时，将中文字符串进行UTF-8编码，然后再写入json对象中。

    Json::Value root;
    Json::Value data;
    constexpr bool shouldUseOldWay = false;
    std::string zhangyubin = "张玉彬";
//    std::string utf8_zhangyubin;
    Json::StreamWriterBuilder builder;
//    utf8_zhangyubin = Json::valueToQuotedStringN(zhangyubin.c_str(), zhangyubin.length(), true);
    root["action"] = zhangyubin;
    data["number"] = 1;
    root["data"] = data;

    if (shouldUseOldWay) {
        Json::FastWriter writer;
        const std::string json_file = writer.write(root);
        std::cout << json_file << std::endl;
    } else {
        const std::string json_file = Json::writeString(builder, root);
        LOGD("json_file = %s", json_file.c_str());
        std::cout << json_file << std::endl;
    }

    return result;
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

//    struct s1 structs1 = {0};
//    struct s2 structs2 = {0};
//    LOGD("[zhang] = sizeof(structs1), %lu", sizeof(structs1));
//    LOGD("[zhang] = sizeof(structs2), %lu", sizeof(structs2));
//    LOGD("[zhang] = sizeof(structs3), %lu", sizeof(struct s3));

    char *a = "; iVBORw0KGgo\n";
    char *b = "; YP2YsSxLlm\n";
    char *c = "; MzAUQ\n";

    // 使用 std::string 类型来拼接字符串
    std::string result =
            std::string(a).substr(2, strlen(a) - 3) + std::string(b).substr(2, strlen(b) - 3) +
            std::string(c).substr(2, strlen(c) - 3);

    LOGD("[zhang] = result = %s", result.c_str());

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
JNIEXPORT jbyteArray JNICALL
Java_com_example_jnistudy_MainActivity_www(JNIEnv *env,
                                           jobject thiz,
                                           jobject on_status_listener) {
    Utils::setStudentName("hello");
    std::unordered_map<std::string, int> a = Utils::getHashMap();
    std::string str = "HELLO";
    const char *c_str = str.c_str();
//=//    jsize len = env->GetStringUTFLength(str);
    jsize len = 5;
    jbyteArray result = env->NewByteArray(len);
    env->SetByteArrayRegion(result, 0, len, (jbyte *) c_str);
//    env->ReleaseStringUTFChars(str, c_str);

    Utils::calculate_crea();
    Utils::calculate_perimeter();
    return result;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_jnistudy_MainActivity_cppJson(JNIEnv *env, jobject thiz) {
    const std::string rawJson = R"({"Age": 200, "Name": "张玉彬"})";
    const auto rawJsonLength = static_cast<int>(rawJson.length());
    constexpr bool shouldUseOldWay = false;
    JSONCPP_STRING err;
    Json::Value root;

    if (shouldUseOldWay) {
        Json::Reader reader;
        reader.parse(rawJson, root);
    } else {
        Json::CharReaderBuilder builder;
        const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
        if (!reader->parse(rawJson.c_str(), rawJson.c_str() + rawJsonLength, &root,
                           &err)) {
            std::cout << "error" << std::endl;
        }
    }
    const std::string name = root["Name"].asString();
    const int age = root["Age"].asInt();
    LOGD("name = %s", name.c_str());
    LOGD("age = %d", age);
    std::cout << name << std::endl;
    std::cout << age << std::endl;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_jnistudy_MainActivity_ji(JNIEnv *env, jobject thiz) {
//    char *s = "adabbccababdd";
//    char *trim_chars = "ab";
//    char *p, *q;
//    p = q = s;
//    while (*p) {
//        if (strchr(trim_chars, *p) == NULL) {
//            *q++ = *p;
//        }
//        p++;
//    }
//    *q = '\0';
//    LOGD("--->[%s], s = %s", __FUNCTION__ , s);

//    std::string s = "adabbccababdd";
//    char a = 'a';
//    s.erase(std::remove(s.begin(), s.end(), a), s.end());
//    LOGD("--->[%s], s = %s", __FUNCTION__ , s.c_str());
//    char b = 'b';
//    s.erase(std::remove(s.begin(), s.end(), b), s.end());
//    LOGD("--->[%s], s = %s", __FUNCTION__ , s.c_str());

//    std::string trim_chars = "ab";
//    std::string result;
//    for (char c : s)
//    {
//        if (trim_chars.find(c) == std::string::npos)
//        {
//            result += c;
//        }
//    }
//
//    LOGD("--->[%s], s = %s", __FUNCTION__ , result.c_str());


    char *test = "abcdefg";
    char *pos;
    pos = strstr(test, "de");
    LOGD("--->[%s], pos = %s", __FUNCTION__, pos);
    pos += sizeof("de") ;
    LOGD("--->[%s], pos = %s", __FUNCTION__, pos);
    printf("hhhhhhhhh");

}