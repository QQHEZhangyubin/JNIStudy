//
// Created by anker on 2023/4/8.
//

#include <string>
#include <utility>
#include <unordered_map>
#include "Utils.h"
#include "Student.h"
#include "Rectangle.h"
#include "Abc.h"
#include "jsoncpp/value.h"

void Utils::setStudentName(std::string name) {
    Student *student = new Student(std::move(name), 25, 1.01f, 1.01, true);
    bool isMale = student->isMale();
    int age = student->getAge();
    std::string nam = student->getName();
    char *mybytes = student->getMyBytes();
    printf("mAge = %s", mybytes);
}

std::unordered_map<std::string, int> Utils::getHashMap() {
    std::unordered_map<std::string, int> unorderedMap;
    unorderedMap.insert({"key1", 1});
    unorderedMap.insert({"key2", 2});
    return unorderedMap;
}

void Utils::calculate_crea() {
    Rectangle *rectangle = new Rectangle(2.0, 7.0);
    double area = rectangle->area();
    printf("area = %f", area);
    Abc::A();
}

void Utils::calculate_perimeter() {
    Rectangle *rectangle = new Rectangle(2.0, 7.0);
    double perimeter = rectangle->perimeter();
    printf("perimeter = %f", perimeter);
}

void Utils::setRectangle(Rectangle rectangle) {
    Json::Value video;
}

Student Utils::getStudent() {
    Student student;
    student.setMName("jjjj");
    return student;
}

Student *Utils::getStudent2() {
    Student *student = new Student();
    student->setMName("hhhh");
    return student;
}





