//
// Created by anker on 2023/4/9.
//

#ifndef JNISTUDY_RECTANGLE_H
#define JNISTUDY_RECTANGLE_H


#include "Shape.h"

class Rectangle : public Shape{
private:
    double width_;
    double height_;
public:
    Rectangle(double w, double h);

    double area() const override;

    double perimeter() const override;

};


#endif //JNISTUDY_RECTANGLE_H
