//
// Created by anker on 2023/4/8.
//

#ifndef JNISTUDY_STUDENT_H
#define JNISTUDY_STUDENT_H
#include <android/log.h>
#define LOG_TAG "atlinweimao"

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

class Student  {
public:
    virtual ~Student();

private:
    std::string mName;
    int mAge;
    bool mIsMale;
    float mScore;
    double mMoney;
public:
    void setMName(const std::string &mName);

public:
    Student(std::string name, int age, float score, double money, bool isMale);

    Student();

    bool isMale() const;

    int getAge() const;

    std::string getName();

    char* getMyBytes();
};

#endif //JNISTUDY_STUDENT_H
