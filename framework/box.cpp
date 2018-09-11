#include <glm/vec3.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/intersect.hpp>
#include <string>
#include <vector>

#include "shape.hpp"
#include "color.hpp"
#include "ray.hpp"
#include "plane.hpp"
#include "box.hpp"
#include "renderer.hpp"


Box::Box(glm::vec3 min, glm::vec3 max, std::string name, std::shared_ptr<Material> material) :
	Shape{ name, material }, min_{ min }, max_{ max }
	{
		boundingBox_ = std::make_shared<BoundingBox>(min_,max_);
	};

Box::~Box() {}

std::ostream& Box::print(std::ostream& os) const {
	return Shape::print(os) << "Min: (" << min_.x << ", " << min_.y << ", " << min_.z << "), Max: (" << max_.x << ", " << max_.y << ", " << max_.z << ")\n";
};

bool Box::intersect(Ray const& ray, float& distance ,glm::vec3& cut_point, glm::vec3& normal, std::shared_ptr<Shape>& shape) const{

	Ray transformedRay = transformRay(world_transformation_inv_,ray);

	//Creating the planes that our box consists of
	Plane plane1{ min_, glm::vec3 { -1,0,0 } };
	Plane plane2{ min_, glm::vec3 { 0,0,-1 } };
	Plane plane3{ min_, glm::vec3 { 0,-1,0 } };
	Plane plane4{ max_, glm::vec3 { 1,0,0 } };
	Plane plane5{ max_, glm::vec3 { 0,0,1 } };
	Plane plane6{ max_, glm::vec3 { 0,1,0 } };

	//calculationg the distance to the plane
	float distance1 = (glm::dot(plane1.normal, plane1.origin) - glm::dot(transformedRay.origin, plane1.normal)) / (glm::dot(transformedRay.direction, plane1.normal));
	float distance2 = (glm::dot(plane2.normal, plane2.origin) - glm::dot(transformedRay.origin, plane2.normal)) / (glm::dot(transformedRay.direction, plane2.normal));
	float distance3 = (glm::dot(plane3.normal, plane3.origin) - glm::dot(transformedRay.origin, plane3.normal)) / (glm::dot(transformedRay.direction, plane3.normal));
	float distance4 = (glm::dot(plane4.normal, plane4.origin) - glm::dot(transformedRay.origin, plane4.normal)) / (glm::dot(transformedRay.direction, plane4.normal));
	float distance5 = (glm::dot(plane5.normal, plane5.origin) - glm::dot(transformedRay.origin, plane5.normal)) / (glm::dot(transformedRay.direction, plane5.normal));
	float distance6 = (glm::dot(plane6.normal, plane6.origin) - glm::dot(transformedRay.origin, plane6.normal)) / (glm::dot(transformedRay.direction, plane6.normal));

	std::vector<glm::vec3> cut_points;
	std::vector<glm::vec3> cut_normals;

	//Checking for each plane if it is in front of the camera
	if (distance1 > 0) {
		cut_point = transformedRay.origin + distance1 * transformedRay.direction;
		if (cut_point.y < max_.y && cut_point.y > min_.y && cut_point.z < max_.z && cut_point.z > min_.z) {
			cut_points.push_back(cut_point);
			cut_normals.push_back(plane1.normal);
		}
	}
	if (distance4 > 0) {
		cut_point = transformedRay.origin + distance4 * transformedRay.direction;
		if (cut_point.y < max_.y && cut_point.y > min_.y && cut_point.z < max_.z && cut_point.z > min_.z) {
			cut_points.push_back(cut_point);
			cut_normals.push_back(plane4.normal);
		}
	}
	//untere Fl�che
	if (distance2 > 0){
		cut_point = transformedRay.origin + distance2 * transformedRay.direction;
		if (cut_point.y < max_.y && cut_point.y > min_.y && cut_point.x < max_.x && cut_point.x > min_.x) {
			cut_points.push_back(cut_point);
			cut_normals.push_back(plane2.normal);
		}
	}
	//obere Fl�che
	if (distance5 > 0) {
		cut_point = transformedRay.origin + distance5 * transformedRay.direction;
		if (cut_point.y < max_.y && cut_point.y > min_.y && cut_point.x < max_.x && cut_point.x > min_.x) {
			cut_points.push_back(cut_point);
			cut_normals.push_back(plane5.normal);
		}
	}
	if (distance3 > 0) {
		cut_point = transformedRay.origin + distance3 * transformedRay.direction;
		if (cut_point.x < max_.x && cut_point.x > min_.x && cut_point.z < max_.z && cut_point.z > min_.z) {
			cut_points.push_back(cut_point);
			cut_normals.push_back(plane3.normal);
		}
	}
	if (distance6 > 0) {
		cut_point = transformedRay.origin + distance6 * transformedRay.direction;
		if (cut_point.x < max_.x && cut_point.x > min_.x && cut_point.z < max_.z && cut_point.z > min_.z) {
			cut_points.push_back(cut_point);
			cut_normals.push_back(plane6.normal);
		}
	}

	//selscting the cut that is closest to the camera
	if (cut_points.size() > 0) {
		glm::vec3 closest_cut = cut_points.at(0);
		glm::vec3 closest_normal = cut_normals.at(0);

		for (auto it = 0; it < cut_points.size(); ++it) {
			if (glm::length(cut_points.at(it) - transformedRay.origin) < glm::length(closest_cut - transformedRay.origin)) {
				closest_cut = cut_points.at(it);
				closest_normal = cut_normals.at(it);
			}
		}
		glm::vec4 transformed_cut = world_transformation_ * glm::vec4{ closest_cut, 1 };
		glm::vec4 transformed_normal = glm::normalize(glm::transpose(world_transformation_inv_) * glm::vec4{ closest_normal , 0 });
		cut_point = glm::vec3{ transformed_cut.x, transformed_cut.y, transformed_cut.z };
		normal = glm::vec3{transformed_normal.x, transformed_normal.y, transformed_normal.z};
		distance = glm::length(cut_point - ray.origin);
		shape = std::make_shared<Box>(min_, max_, name(), material());
		return true;
	}
	return false;
};

glm::vec3 Box::min() const {
	return min_;
}

glm::vec3 Box::max() const {
	return max_;
}



