#ifndef SHAPE_HEADER
#define SHAPE_HEADER
#include "color.hpp"
#include "ray.hpp"
#include <string>
#include <iostream>

class Shape {
public:
	Shape(std::string name, Color color);
	~Shape();
	virtual double area() const = 0;
	virtual double volume() const = 0;
	virtual std::ostream& print(std::ostream& os) const;
	virtual bool intersect(Ray const&, float&) const = 0;
	std::string name() const;
	Color color() const;	
private:
	std::string name_;
	Color color_;
};

std::ostream& operator <<(std::ostream& os, Shape const& s);
#endif