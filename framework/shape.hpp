#ifndef SHAPE_HEADER
#define SHAPE_HEADER
class Shape {
public:
	Shape() {};
	virtual double area() const = 0;
	virtual double volume() const = 0;
};
#endif