#ifndef SHAPE_HEADER
#define SHAPE_HEADER
#include "color.hpp"
class Shape {
public:
	Shape(std::string name, Color color) : name_{ name }, color_{color} {};
	virtual double area() const = 0;
	virtual double volume() const = 0;
	std::string name() const { return name_; };
	Color color() const { return color_; };
private:
	std::string name_;
	Color color_;
};
#endif