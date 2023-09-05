#include <jni.h>
#include <string>
#include <iostream> //To change the formatting of std::cerr.
#include <signal.h> //For floating point exceptions.
#if defined(__linux__) || (defined(__APPLE__) && defined(__MACH__))
#include <sys/resource.h> //For setpriority.
#endif
#include <fstream>
#include <cstring>
#include <vector>
#include <locale>         // std::wstring_convert
#include <codecvt>        // std::codecvt_utf8
#include <stdlib.h>
#include"sliceservice.h"
#include <stdio.h>
#include <iostream>
#include <thread>
//#include "src/Application.h"
#include "AOsLog.h"
#if defined(_WIN32)
#include <windows.h>
#include <Dbghelp.h>
#endif


//extern "C"
//JNIEXPORT jint JNICALL
//Java_com_example_nativelib_CuraEngineLib_ankerTest(JNIEnv *env,
//                                                   jobject thiz,
//                                                   jstring name,
//                                                   jstring path) {
//    char modelSTLPath[256];
//    char modelGcodePath[256];
//
//    std::string modelName = env->GetStringUTFChars(name, 0);
//    std::string pathh =  env->GetStringUTFChars(path, 0);
//    std::string  a = pathh + "/model/%s.stl";
//
//    std::string  b = "/storage/emulated/0/Download/ankerOut/%s.gcode";
////    sprintf(modelSTLPath, "/mnt/sdcard/Android/data/com.example.ms/model/%s.stl", modelName.c_str());
//    sprintf(modelSTLPath, a.c_str(), modelName.c_str());
////    sprintf(modelGcodePath, "/mnt/sdcard/Android/data/com.example.ms/output/%s.gcode", modelName.c_str());
//    sprintf(modelGcodePath, b.c_str(), modelName.c_str());
//    std::string pathh1 = pathh + "/definitions/fdmprinter.def.json";
//    std::string pathh2 = pathh + "/definitions/fdmextruder.def.json";
//
//    const char* myargv[13] = {"CuraEngine", "slice", "-v", "-j", pathh1.c_str(), "-v", "-j", pathh2.c_str(),"-o", modelGcodePath,"-e1","-l", modelSTLPath};
//    int myargc = 13;
////    using namespace cura;
////    cura::Application::getInstance().run(myargc, (char**)myargv);
//    return 0;
//}

void progress(HANDLE handle, float progress)
{
    std::cout << __FUNCTION__ << ":" << progress << std::endl;
    OS_LOG("------>>【%s】 progress = %f", __FUNCTION__, progress);
}//�������

void message(HANDLE handle, const char* msg)
{
    //std::cout << __FUNCTION__;
    //std::cout << msg << std::endl;

}//��Ƭ��־���

void failed(HANDLE handle, int errorCode) {
    std::cout << __FUNCTION__ << ":" << errorCode << std::endl;
    OS_LOG("------>>【%s】 errorCode = %d", __FUNCTION__, errorCode);
}//��Ƭʧ��

void successed(HANDLE handle, SliceResuslt result) {
    std::cout << __FUNCTION__ << ":" << "OK" << std::endl;
    OS_LOG("------>>【%s】 OK", __FUNCTION__);
}//��Ƭ�ɹ�

void Slice(int index, std::string modelName, std::string modelPath, std::string configPath)
{
    OS_LOG("------>>【%s】 modelName = %s, modelPath = %s, configPath = %s", __FUNCTION__, modelName.c_str(), modelPath.c_str(), configPath.c_str());

    float matrix0[16] =
            {
                    1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1
            };
    SliceResuslt result;

    AKSS_RegisterSucessedCallback(successed);
    AKSS_RegisterFailedCallback(failed);
    AKSS_RegisterMessageCallback(message);
    AKSS_RegisterProgressCallback(progress);

    HANDLE handle = AKSS_CreateInstance(configPath.c_str());

    NEXT_SLICE:

//    AKSS_SetWorkPath(handle, "./models");
    AKSS_SetWorkPath(handle, modelPath.c_str());
//    AKSS_AddMeshByFile(handle, "13����ĸ.stl", matrix0);
    AKSS_AddMeshByFile(handle, modelName.c_str(), matrix0);

    AKSS_SetParam(handle, "meta_current_machine_name", "AnkerMake M5");//��������
    AKSS_SetParam(handle, "machine_nozzle_size", "0.40"); //�����С
    AKSS_SetParam(handle, "meta_current_material_name", "ABS"); //��������
    AKSS_SetParam(handle, "param_print_mode", "fast"); //��ӡģʽ

    int flag = AKSS_StartSlice(handle);
    OS_LOG("------>>【%s】 flag = %d", __FUNCTION__, flag);

#if 1
    bool userCancelSlice = false;
    while (AKSS_IsSlicing(handle))
    {
        if (userCancelSlice)
        {
            AKSS_StopSlice(handle);
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

#else

    AKSS_Wait(handle, -1); //<0 infinite wait

#endif

    AKSS_DestoryInstance(handle);

    std::cout << __FUNCTION__ << index << std::endl;
}

#define AKSS_MAX_THREAS 1

int main(int argc, char* argv[])
{
    std::thread t[AKSS_MAX_THREAS];

//    for (int i = 0; i < AKSS_MAX_THREAS; i++) {
//        t[i] = std::thread(Slice, i);
//    }
//
//    for (int i = 0; i < AKSS_MAX_THREAS; i++) {
//        t[i].join();
//    }

    std::cout << "thread ending" << std::endl;

    return 0;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_example_nativelib_CuraEngineLib_nativeSlice(JNIEnv *env, jobject thiz,
                                                     jstring stl_name,
                                                     jstring stl_absolute_path,
                                                     jstring config_path) {
    std::thread t[AKSS_MAX_THREAS];
    std::string modelName = env->GetStringUTFChars(stl_name, 0);
    std::string modelPath = env->GetStringUTFChars(stl_absolute_path, 0);
    std::string configPath = env->GetStringUTFChars(config_path, 0);
    for (int i = 0; i < AKSS_MAX_THREAS; i++) {
        t[i] = std::thread(Slice, i, modelName, modelPath, configPath);
    }


    for (int i = 0; i < AKSS_MAX_THREAS; i++) {
        t[i].join();
    }


    std::cout << "thread ending" << std::endl;
    OS_LOG("------>>【%s】 thread ending", __FUNCTION__);
    return 0;
}