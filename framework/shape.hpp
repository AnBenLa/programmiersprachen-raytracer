#ifndef SHAPE_HEADER
#define SHAPE_HEADER
#include "color.hpp"
#include <string>
#include <iostream>

class Shape {
public:
	Shape(std::string name, Color color) : name_{ name }, color_{ color } { std::cout << "Base constructor\n"; };
	~Shape() { std::cout << "Base destructor\n";  };
	virtual double area() const = 0;
	virtual double volume() const = 0;
	virtual std::ostream& print(std::ostream& os) const {
		return os << "Shape name: " << name_ << " Shape color: " << color_;
	};
	std::string name() const { return name_; };
	Color color() const { return color_; };
	friend std::ostream& operator <<(std::ostream& os, Shape const& s)
	{
		return s.print(os);
	}
private:
	std::string name_;
	Color color_;
};


#endif