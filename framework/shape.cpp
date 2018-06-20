#include "shape.hpp"
#include "color.hpp"
#include <iostream>
#include <string>

Shape::Shape(std::string name, Color color) : name_{ name }, color_{ color } { std::cout << "Base constructor\n"; };

Shape::~Shape() { std::cout << "Base destructor\n"; };

std::ostream& Shape::print(std::ostream& os) const 
{ return os << "Shape name: " << name_ << " Shape color: " << color_; };

std::string Shape::name() const { return name_; };

Color Shape::color() const { return color_; };

std::ostream& operator <<(std::ostream& os, Shape const& s)
{	return s.print(os); }