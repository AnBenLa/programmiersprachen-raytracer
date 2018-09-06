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
#include "renderer.hpp"

Sphere::Sphere(glm::vec3 center, double radius, std::string name, std::shared_ptr<Material> material) :
	Shape{name, material}, center_ { center }, radius_{ radius } 
	{
		calculateBoundingBox();
	};

Sphere::~Sphere() {};


std::ostream& Sphere::print(std::ostream& os) const {
	return Shape::print(os) << "Center: (" << center_.x << ", " << center_.y << ", " << center_.z << "), Radius: (" << radius_ << ")\n";
};

bool Sphere::intersect(Ray const& ray, float& distance ,glm::vec3& cut_point, glm::vec3& normal, std::shared_ptr<Shape>& shape) const {
	//std::cout<<"\n\n\n"<<ray.origin.x<<","<<ray.origin.y<<","<<ray.origin.z<<"\n";
	Ray transformedRay = transformRay(world_transformation_inv_,ray);
	//std::cout<<transformedRay.origin.x<<","<<transformedRay.origin.y<<","<<transformedRay.origin.z<<"\n";
	bool hit_found = glm::intersectRaySphere(transformedRay.origin, glm::normalize(transformedRay.direction), center_, radius_, cut_point , normal);
	if (hit_found) {
		glm::vec4 transformed_cut = world_transformation_ * glm::vec4{ cut_point, 1 };
		glm::vec4 transformed_normal = glm::normalize(glm::transpose(world_transformation_inv_) * glm::vec4{ normal , 0 });
		cut_point = glm::vec3{ transformed_cut.x, transformed_cut.y, transformed_cut.z };
		normal = glm::vec3{ transformed_normal.x, transformed_normal.y, transformed_normal.z };
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

    boundingBox_= std::make_shared<BoundingBox>(minBbox,maxBbox);
}

std::shared_ptr<BoundingBox> Sphere::boundingBox()const {
	return boundingBox_;
}