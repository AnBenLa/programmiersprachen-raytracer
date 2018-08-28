#include <iostream>
#include <string>

#include "shape.hpp"
#include "color.hpp"


Shape::Shape(std::string const& name, std::shared_ptr<Material> const& material) : name_{ name }, material_{ material}, boundingBox_{nullptr} {};

Shape::~Shape() {};

std::ostream& Shape::print(std::ostream& os) const 
{ return os << "Shape name: " << name_ << " Shape material: " << *material_; };

std::string Shape::name() const { return name_; };

std::shared_ptr<Material> Shape::material() const { return material_; };

std::ostream& operator <<(std::ostream& os, Shape const& s)
{	return s.print(os); }