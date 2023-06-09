//
// Created by anker on 2023/4/8.
//

#ifndef JNISTUDY_UTILS_H
#define JNISTUDY_UTILS_H

#include "Rectangle.h"

class Utils {
protected:
    void setRectangle(Rectangle rectangle);
public:
    static void setStudentName(std::string name);
    static std::unordered_map<std::string, int> getHashMap();
    static void calculate_crea();
    static void calculate_perimeter();
};


#endif //JNISTUDY_UTILS_H
