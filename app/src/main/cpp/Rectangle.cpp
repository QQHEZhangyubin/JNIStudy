//
// Created by anker on 2023/4/9.
//

#include "Rectangle.h"
#include <string>

double Rectangle::area() const {
    return width_ * height_;
}

double Rectangle::perimeter() const {
    return 2 * (width_ + height_);
}

Rectangle::Rectangle(double w, double h) {
    width_ = w;
    height_ = h;
}

