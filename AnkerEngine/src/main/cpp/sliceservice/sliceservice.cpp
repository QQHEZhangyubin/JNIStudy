#include "sliceservice.h"
#include "AnkerCloudSlice.h"

const int MAX_INSTANCE_COUNT = 1;
static int g_nIntanceCount = 0;

SLICE_SERVICE_API HANDLE AKSS_CreateInstance(const char* configPath)
{
	if (g_nIntanceCount >= MAX_INSTANCE_COUNT)
	{
		return NULL;
	}

	++g_nIntanceCount;
	return (HANDLE)(new akss::AnkerCloudSlicer(configPath));
}

//before slice 
SLICE_SERVICE_API int AKSS_SetWorkPath(HANDLE handle, const char* path)
{
	if (handle == nullptr) {
		return -1;
	}

	return static_cast<akss::AnkerCloudSlicer*>(handle)->setWorkPath(path);
}

SLICE_SERVICE_API int AKSS_SetParam(HANDLE handle, const char* key, const char* value)
{
	if (handle == nullptr) {
		return -1;
	}

	return static_cast<akss::AnkerCloudSlicer*>(handle)->setParam(key, value)? 0 : -2;
}

//SLICE_SERVICE_API int AKSS_AddMeshByData(HANDLE handle, const float* vertics, int size)
//{
//	if (handle == nullptr) {
//		return -1;
//	}
//
//	return static_cast<akss::AnkerCloudSlicer*>(handle)->addMesh(vertics, size) ? 0 : -2;
//}

SLICE_SERVICE_API int AKSS_AddMeshByFile(HANDLE handle, const char* fileName, const float* matrix)
{
	if (handle == nullptr) {
		return -1;
	}

	return static_cast<akss::AnkerCloudSlicer*>(handle)->addMesh(fileName, matrix) ? 0 : -2;
}

//before slice 
SLICE_SERVICE_API void AKSS_RegisterProgressCallback(PROGRESS_CALLBACK callback)
{
	akss::AnkerCloudSlicer::progressCallback = callback;
}

SLICE_SERVICE_API void AKSS_RegisterMessageCallback(MESSAGE_CALLBACK callback)
{
	akss::AnkerCloudSlicer::messageCallback = callback;
}

SLICE_SERVICE_API void AKSS_RegisterFailedCallback(FAILED_CALLBACK callback)
{
	akss::AnkerCloudSlicer::failedCallback = callback;
}

SLICE_SERVICE_API void AKSS_RegisterSucessedCallback(SUCESSED_CALLBACK callback)
{
	akss::AnkerCloudSlicer::sucessedCallback = callback;
}

//ready,can use
SLICE_SERVICE_API int AKSS_StartSlice(HANDLE handle)
{
	if (handle == nullptr) {
		return -1;
	}

	return static_cast<akss::AnkerCloudSlicer*>(handle)->startSlice() ? 0 : -2;
}

SLICE_SERVICE_API int AKSS_IsSlicing(HANDLE handle)
{
	if (handle == nullptr) {
		return -1;
	}

	return static_cast<akss::AnkerCloudSlicer*>(handle)->isSlicing() ? 1 : 0;
}

SLICE_SERVICE_API int AKSS_StopSlice(HANDLE handle)
{
	if (handle == nullptr) {
		return -1;
	}

	return static_cast<akss::AnkerCloudSlicer*>(handle)->stopSlice() ? 0 : -2;
}

SLICE_SERVICE_API int AKSS_Wait(HANDLE handle, int msTimout)
{
	if (handle == nullptr) {
		return -1;
	}

	return static_cast<akss::AnkerCloudSlicer*>(handle)->waitSliceEnd(msTimout) ? 0 : -2;
}

SLICE_SERVICE_API int AKSS_DestoryInstance(HANDLE handle)
{
	if (handle == nullptr) {
		return -1;
	}

	--g_nIntanceCount;
	auto pInstance = static_cast<akss::AnkerCloudSlicer*>(handle);
	pInstance->stopSlice();
	delete pInstance;

	return 0;
}

