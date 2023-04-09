//
// Created by anker on 2023/4/8.
//

#include <string>
#include "Student.h"


Student::Student(std::string name, int age, float score, double money, bool isMale) {
    mName = name;
    mAge = age;
    mIsMale = isMale;
    mScore = score;
    mMoney = money;
}

bool Student::isMale() const {
    return mIsMale;
}

int Student::getAge() const {
    try {
        throw std::runtime_error("Something went wrong");
    } catch (const std::exception& exception) {
       std::string(exception.what());
    };
    return mAge;
}

std::string Student::getName() {

    std::transform(mName.begin(),
                   mName.end(),
                   mName.begin(),
                   [](unsigned char c) {
                       return std::toupper(c);
                   });
    return mName;
}

char* Student::getMyBytes() {
    const char* mychar = mName.c_str();
    char* mybytes = new char[mName.length()];
    std::memcpy(mybytes, mychar, mName.length());
    mybytes[mName.length()] = '\0';
    return mybytes;
}
