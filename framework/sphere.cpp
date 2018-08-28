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


std::ostream& Sphere::print(std::ostream& os) const {
	return Shape::print(os) << "Center: (" << center_.x << ", " << center_.y << ", " << center_.z << "), Radius: (" << radius_ << ")\n";
};

bool Sphere::intersect(Ray const& ray, float& distance ,glm::vec3& cut_point, glm::vec3& normal, std::shared_ptr<Shape> shape) const {
	bool hit_found = glm::intersectRaySphere(ray.origin, glm::normalize(ray.direction), center_, radius_, cut_point , normal);
	if (hit_found) {
		distance = glm::length(cut_point - ray.origin);
		shape = std::make_shared<Sphere>(center_, radius_, name(), material());
		return true;
	}

	return false;
}

glm::vec3 Sphere::center() const { return center_; };

double Sphere::radius() const { return radius_; };

void Sphere::calculateBoundingBox()
{
	glm::vec3 minBbox{center_.x-radius_,center_.y-radius_,center_.z-radius_};
    glm::vec3 maxBbox{center_.x+radius_,center_.y+radius_,center_.z+radius_};

    boundingBox_= std::make_shared<Box>(minBbox,maxBbox);
}

std::shared_ptr<BoundingBox> Sphere::boundingBox()const {
	return boundingBox_;
}