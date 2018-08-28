#include <glm/vec3.hpp>
#include <glm/gtx/norm.hpp>
#include <string>
#include <algorithm>

#include "shape.hpp"
#include "color.hpp"
#include "ray.hpp"
#include "cone.hpp"
#include "box.hpp"


Cone::Cone(glm::vec3 base, glm::vec3 peak, float radius ,std::string name, std::shared_ptr<Material> material) :
	Shape{ name, material }, base_{ base }, peak_{ peak }, radius_{radius} 
	{
		calculateBoundingBox();
	};

Cone::~Cone() {}

std::ostream& Cone::print(std::ostream& os) const {
	return Shape::print(os) << "Base: (" << base_.x << ", " << base_.y << ", " << base_.z << "), Peak: (" << peak_.x << ", " << peak_.y << ", " << peak_.z << "), Radius: "<< radius_ <<"\n";
};

bool Cone::intersect(Ray const& ray, float& distance ,glm::vec3& cut_point, glm::vec3& normal, std::shared_ptr<Shape> shape) const {
	
	glm::vec3 v = ray.direction;
	glm::vec3 h = base_ - peak_;
	glm::vec3 h_norm = glm::normalize(h);
	glm::vec3 w = ray.origin - peak_;
	float m = (pow(radius_,2)) / pow(glm::length(h),2);
	float v_h = glm::dot(ray.direction, h_norm);
	float w_h = glm::dot(w, h_norm);

	float a = glm::dot(v,v) - m*pow(v_h,2) - pow(v_h, 2);
	float b = 2*(glm::dot(v,w) - m*v_h*w_h - v_h*w_h);
	float c = glm::dot(w,w) - m*pow(w_h,2) - pow(w_h,2);

	if (pow(b, 2) - 4 * a*c >= 0) {
		float t_1 = (-b + sqrt(pow(b, 2) - 4 * a*c)) / (2 * a);
		float t_2 = (-b - sqrt(pow(b, 2) - 4 * a*c)) / (2 * a);
		if (t_2 < t_1 && t_2 > 0) {
			t_1 = t_2;
		}
		if (pow(b, 2) - 4 * a*c == 0) {
			t_1 = -b / (2 * a);
		}
		
		cut_point = ray.origin + t_1 * glm::normalize(ray.direction);
		
		if (0 < glm::dot(cut_point - peak_, h) && glm::length(peak_-cut_point) <= sqrt(pow(radius_,2) + pow(glm::length(h),2))) {
			//normal not correct yet
			float r = sqrt(pow(cut_point.x - base_.x, 2) + pow(cut_point.z - base_.z, 2));
			float z = r * (radius_ / glm::length(h));
			glm::vec3 p = glm::vec3{ cut_point.x - base_.x, z, cut_point.z - base_.z };
			normal = glm::normalize(p);
			distance = glm::length(cut_point - ray.origin);
			shape = std::make_shared<Shape>(this);
			return true;
		}
		return false;

	} 
	return false;
};

glm::vec3 Cone::peak() const {
	return peak_;
}

glm::vec3 Cone::base() const {
	return base_;
}

float Cone::radius() const {
	return radius_;
}

void Cone::calculateBoundingBox()
{
	glm::vec3 minBbox{base_.x-radius_,base_.y-radius_,base_.z-radius_};
    glm::vec3 maxBbox{peak_.x+radius_,peak_.y+radius_,peak_.z+radius_};

    boundingBox_= std::make_shared<BoundingBox>(minBbox,maxBbox);
}

std::shared_ptr<BoundingBox> Cone::boundingBox() const{
	return boundingBox_;
}



