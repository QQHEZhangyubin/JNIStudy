#pragma once
typedef struct _tagSliceResuslt
{
	unsigned long int printTime; // unit: s
	double filamentLen;			 // Estimated material consumption length£¬unit:m
	double filamentWeight;       // Estimated material consumption length  unit:g
	unsigned long int layerSize; // slice total layer size
	double maxSpeed;   //unit:mm/s
	double x;   // x size unit:mm
	double y;   // y size unit:mm
	double z;   // z size unit:mm
	int reserve[100];
}SliceResuslt, * LPSliceResuslt;

typedef void (*PROGRESS_CALLBACK)(void* handle, float progress);		//progess notify
typedef void (*MESSAGE_CALLBACK)(void* handle, const char* message);	//message notify
typedef void (*FAILED_CALLBACK)(void* handle, int errorCode);			//failed notify
typedef void (*SUCESSED_CALLBACK)(void* handle, SliceResuslt result);	//sucessed notify