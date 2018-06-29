#ifndef SHAPE_HEADER
#define SHAPE_HEADER
#include "color.hpp"
#include "ray.hpp"
#include "material.hpp"
#include <string>
#include <iostream>

class Shape {
public:
	Shape(std::string name, std::shared_ptr<Material> material);
	~Shape();
	virtual double area() const = 0;
	virtual double volume() const = 0;
	virtual std::ostream& print(std::ostream& os) const;
	virtual bool intersect(Ray const&, float&) const = 0;
	std::string name() const;
	std::shared_ptr<Material> material() const;
private:
	std::string name_;
	std::shared_ptr<Material> material_;
};

std::ostream& operator <<(std::ostream& os, Shape const& s);
#endif