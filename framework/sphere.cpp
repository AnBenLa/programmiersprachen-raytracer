#define _USE_MATH_DEFINES
#include "sphere.hpp"
#include <glm/vec3.hpp>
#include <math.h>
#include "shape.hpp"
#include "color.hpp"
#include <string>
#include <glm/gtx/intersect.hpp>
#include "ray.hpp"

Sphere::Sphere(glm::vec3 center, double radius, std::string name, std::shared_ptr<Material> material) :
	Shape{name, material}, center_ { center }, radius_{ radius } {};

Sphere::~Sphere() {};

double Sphere::area() const{
	return 4 * M_PI * pow(radius_,2);
};

double Sphere::volume() const{
	return (4.0 / 3.0) * M_PI * pow(radius_, 3);
};

std::ostream& Sphere::print(std::ostream& os) const {
	return Shape::print(os) << "Center: (" << center_.x << ", " << center_.y << ", " << center_.z << "), Radius: (" << radius_ << ")\n";
};

bool Sphere::intersect(Ray const& ray, float& distance, glm::vec3& cut_point, glm::vec3& normal) const {
	bool status = glm::intersectRaySphere(ray.origin, glm::normalize(ray.direction), center_, radius_, cut_point , normal);
	distance = glm::length(cut_point - ray.origin);
	return status;
}

glm::vec3 Sphere::center() const { return center_; };

double Sphere::radius() const { return radius_; };