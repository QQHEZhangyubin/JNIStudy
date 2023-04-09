//
// Created by anker on 2023/4/8.
//

#ifndef JNISTUDY_STUDENT_H
#define JNISTUDY_STUDENT_H

class Student  {
private:
    std::string mName;
    int mAge;
    bool mIsMale;
    float mScore;
    double mMoney;

public:
    Student(std::string name, int age, float score, double money, bool isMale);

    bool isMale() const;

    int getAge() const;

    std::string getName();

    char* getMyBytes();
};

#endif //JNISTUDY_STUDENT_H
