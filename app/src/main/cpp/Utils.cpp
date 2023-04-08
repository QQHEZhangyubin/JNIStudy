//
// Created by anker on 2023/4/8.
//

#include <string>
#include <utility>
#include <unordered_map>
#include "Utils.h"
#include "Student.h"

void Utils::setStudentName(std::string name) {
    Student *student = new Student(std::move(name), 25, 1.01f, 1.01, true);
    bool isMale = student->isMale();
    int age = student->getAge();
    std::string nam = student->getName();
    printf("mAge = %d", age);
}

std::unordered_map<std::string, int> Utils::getHashMap() {
    std::unordered_map<std::string, int> unorderedMap;
    unorderedMap.insert({"key1", 1});
    unorderedMap.insert({"key2", 2});
    return unorderedMap;
};



