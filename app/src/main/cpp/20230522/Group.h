//
// Created by anker on 2023/5/22.
//

#ifndef JNISTUDY_GROUP_H
#define JNISTUDY_GROUP_H

#include <iostream>

using namespace std;

class Mem {
public:
    Mem();
    Mem(int i);
private:
    int num = 2; // 使用= 号初始化非静态成员
};

class Group {
public:
    Group();
    Group(int i);
    void NumOfA();
    void NumOfB();

private:
    char val{'g'};
    Mem a;
    Mem b{19};
};


#endif //JNISTUDY_GROUP_H
