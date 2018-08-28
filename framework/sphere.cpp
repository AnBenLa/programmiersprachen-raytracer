#define _USE_MATH_DEFINES
#include "sphere.hpp"
#include <glm/vec3.hpp>
#include <math.h>
#include "shape.hpp"
#include "color.hpp"
#include <string>
#include <glm/gtx/intersect.hpp>
#include "ray.hpp"
#include "box.hpp"

Sphere::Sphere(glm::vec3 center, double radius, std::string name, std::shared_ptr<Material> material) :
	Shape{name, material}, center_ { center }, radius_{ radius } 
	{
		calculateBoundingBox();
	};

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

std::shared_ptr<Hit> Sphere::intersect(Ray const& ray, glm::vec3& cut_point, glm::vec3& normal) const {
	std::shared_ptr<Hit> hit = nullptr;
	bool hit_found = glm::intersectRaySphere(ray.origin, glm::normalize(ray.direction), center_, radius_, cut_point , normal);
	if (hit_found) {
		float distance = glm::length(cut_point - ray.origin);
		hit = std::make_shared<Hit>(ray.direction, cut_point, normal, std::make_shared<Shape>(this), distance);
	}

	return hit;
}

glm::vec3 Sphere::center() const { return center_; };

double Sphere::radius() const { return radius_; };

void Sphere::calculateBoundingBox()
{
	glm::vec3 minBbox{center_.x-radius_,center_.y-radius_,center_.z-radius_};
    glm::vec3 maxBbox{center_.x+radius_,center_.y+radius_,center_.z+radius_};

    boundingBox_= std::make_shared<Box>(minBbox,maxBbox,name()+"BoundingBox",nullptr);
}