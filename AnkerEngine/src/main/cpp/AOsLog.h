//
// Created by anker on 2023/4/27.
//

#ifndef FDMPRINT_AOSLOG_H
#define FDMPRINT_AOSLOG_H

#define LOGTAG "d3native"

#ifdef __ANDROID__

#include <android/log.h>

#define OS_LOG(format, ...) __android_log_print(ANDROID_LOG_DEBUG, LOGTAG, format, ##__VA_ARGS__)
#else
#include <stdio.h>
#define OS_LOG(format, ...) printf("[%s] " format "\n", LOGTAG, ##__VA_ARGS__)
#endif


#endif //FDMPRINT_AOSLOG_H
