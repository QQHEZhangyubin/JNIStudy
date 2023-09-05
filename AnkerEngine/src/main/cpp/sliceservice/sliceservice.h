#ifndef _INTERFACE_EXPORT_H
#define _INTERFACE_EXPORT_H

#ifdef _WIN32
#ifdef SLICE_SERVICE_DLL
#define SLICE_SERVICE_API __declspec(dllexport)
#else
#define SLICE_SERVICE_API __declspec(dllimport)
#endif
#else
#ifdef SLICE_SERVICE_DLL
#define SLICE_SERVICE_API __attribute__((visibility("default")))
#else
#define SLICE_SERVICE_API
#endif
#endif

#ifndef HANDLE
#define HANDLE void*
#endif

#include "typedef.h"

/* Noet
An instance can be sliced multiple times, but only one slicing task can be created.
The slicing task must be performed after the previous task stops. Otherwise, 
the AKSS_CreateInstance call is required to create a new instance
*/

#ifdef __cplusplus
extern "C" {
#endif

	//first
	SLICE_SERVICE_API void AKSS_RegisterProgressCallback(PROGRESS_CALLBACK callback);
	SLICE_SERVICE_API void AKSS_RegisterMessageCallback(MESSAGE_CALLBACK callback);
	SLICE_SERVICE_API void AKSS_RegisterFailedCallback(FAILED_CALLBACK callback);
	SLICE_SERVICE_API void AKSS_RegisterSucessedCallback(SUCESSED_CALLBACK callback);

	/*
	* configPath: Path of config files
	*/
	SLICE_SERVICE_API HANDLE AKSS_CreateInstance(const char* configPath);//

	//before slice
	/*
	* parameter setings and CuraEngine
	* error: < 0 sucessed : 0
	*/
	SLICE_SERVICE_API int AKSS_SetWorkPath(HANDLE handle, const char* workPath);//

	/*
	* etc:machine nozzle meterial printmode
	* error: < 0 sucessed : 0
	*/
	SLICE_SERVICE_API int AKSS_SetParam(HANDLE handle, const char* key, const char* value);

	///*
	//* 1 face include 9 size error:<0 sucessed : 0
	//* error: < 0 sucessed : 0
	//*/
	//SLICE_SERVICE_API int AKSS_AddMeshByData(HANDLE handle, const float* vertics, int size);

	/*
	* 3*3 transform matrix
	* only STL error:<0 sucessed : 0
	* error: < 0 sucessed : 0
	*/
	SLICE_SERVICE_API int AKSS_AddMeshByFile(HANDLE handle, const char* fileName, const float* matrix);

	//ready,can use
	SLICE_SERVICE_API int AKSS_StartSlice(HANDLE handle);//error:<0 sucessed : 0
	SLICE_SERVICE_API int AKSS_IsSlicing(HANDLE handle); //query current slice status, error:<0 runing:1 stop:0
	SLICE_SERVICE_API int AKSS_StopSlice(HANDLE handle); //error:<0 sucessed : 0
	SLICE_SERVICE_API int AKSS_Wait(HANDLE handle, int msTimout); //error:<0 sucessed : 0

	/*
	* end,release rsource
	*/
	SLICE_SERVICE_API int AKSS_DestoryInstance(HANDLE handle);

#ifdef __cplusplus
}
#endif

#endif // _INTERFACE_EXPORT_H

