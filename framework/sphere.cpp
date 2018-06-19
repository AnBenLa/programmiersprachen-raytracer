#define _USE_MATH_DEFINES
#include "sphere.hpp"
#include <glm/vec3.hpp>
#include <math.h>
#include "shape.hpp"
#include "color.hpp"
#include <string>
#include <glm/gtx/intersect.hpp>
#include "ray.hpp"

Sphere::Sphere(glm::vec3 center, double radius, std::string name, Color color) : 
	Shape{name, color}, center_ { center }, radius_{ radius } {};

double Sphere::area() const{
	return 4 * M_PI * pow(radius_,2);
};

double Sphere::volume() const{
	return (4.0 / 3.0) * M_PI * pow(radius_, 3);
};

std::ostream& Sphere::print(std::ostream& os) const {
	return Shape::print(os) << "Shape form: Sphere";
};

//Worauf bezieht sich die Distance?
bool Sphere::intersect(Ray const& ray) const {
	float distance = 0.0f;
	return glm::intersectRaySphere(ray.origin, ray.direction, center_, radius_ * radius_, distance);
}

glm::vec3 Sphere::center() const { return center_; };

double Sphere::radius() const { return radius_; };