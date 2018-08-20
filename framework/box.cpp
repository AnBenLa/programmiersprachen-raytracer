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


Box::Box(glm::vec3 min, glm::vec3 max, std::string name, std::shared_ptr<Material> material) :
	Shape{ name, material }, min_{ min }, max_{ max } {};

Box::~Box() {}

double Box::area() const{
	double x = max_.x - min_.x;
	double y = max_.y - min_.y;
	double z = max_.z - min_.z;
	return 2*x*y + 2*x*z + 2*y*z;
};

double Box::volume() const{
	double x = max_.x - min_.x;
	double y = max_.y - min_.y;
	double z = max_.z - min_.z;
	return x*y*z;
};

std::ostream& Box::print(std::ostream& os) const {
	return Shape::print(os) << "Min: (" << min_.x << ", " << min_.y << ", " << min_.z << "), Max: (" << max_.x << ", " << max_.y << ", " << max_.z << ")\n";
};

bool Box::intersect(Ray const& ray, float& t, glm::vec3& cut_point, glm::vec3& normal) const{
	
	//Creating the planes that our box consists of
	Plane plane1{ min_, glm::vec3 { 1,0,0 } };
	Plane plane2{ min_, glm::vec3 { 0,0,1 } };
	Plane plane3{ min_, glm::vec3 { 0,1,0 } };
	Plane plane4{ max_, glm::vec3 { 1,0,0 } };
	Plane plane5{ max_, glm::vec3 { 0,0,1 } };
	Plane plane6{ max_, glm::vec3 { 0,1,0 } };

	//calculationg the distance to the plane
	float distance1 = (glm::dot(plane1.normal, plane1.origin) - glm::dot(ray.origin, plane1.normal)) / (glm::dot(ray.direction, plane1.normal));
	float distance2 = (glm::dot(plane2.normal, plane2.origin) - glm::dot(ray.origin, plane2.normal)) / (glm::dot(ray.direction, plane2.normal));
	float distance3 = (glm::dot(plane3.normal, plane3.origin) - glm::dot(ray.origin, plane3.normal)) / (glm::dot(ray.direction, plane3.normal));
	float distance4 = (glm::dot(plane4.normal, plane4.origin) - glm::dot(ray.origin, plane4.normal)) / (glm::dot(ray.direction, plane4.normal));
	float distance5 = (glm::dot(plane5.normal, plane5.origin) - glm::dot(ray.origin, plane5.normal)) / (glm::dot(ray.direction, plane5.normal));
	float distance6 = (glm::dot(plane6.normal, plane6.origin) - glm::dot(ray.origin, plane6.normal)) / (glm::dot(ray.direction, plane6.normal));;

	std::vector<glm::vec3> cut_points;
	std::vector<glm::vec3> cut_normals;

	//Checking for each plane if it is in front of the camera
	if (distance1 > 0) {
		cut_point = ray.origin + distance1 * ray.direction;
		if (cut_point.y < max_.y && cut_point.y > min_.y && cut_point.z < max_.z && cut_point.z > min_.z) {
			cut_points.push_back(cut_point);
			cut_normals.push_back(plane1.normal);
		}
	}
	if (distance4 > 0) {
		cut_point = ray.origin + distance4 * ray.direction;
		if (cut_point.y < max_.y && cut_point.y > min_.y && cut_point.z < max_.z && cut_point.z > min_.z) {
			cut_points.push_back(cut_point);
			cut_normals.push_back(plane4.normal);
		}
	}
	//untere Fläche
	if (distance2 > 0){
		cut_point = ray.origin + distance2 * ray.direction;
		if (cut_point.y < max_.y && cut_point.y > min_.y && cut_point.x < max_.x && cut_point.x > min_.x) {
			cut_points.push_back(cut_point);
			cut_normals.push_back(plane2.normal);
		}
	}
	//obere Fläche
	if (distance5 > 0) {
		cut_point = ray.origin + distance5 * ray.direction;
		if (cut_point.y < max_.y && cut_point.y > min_.y && cut_point.x < max_.x && cut_point.x > min_.x) {
			cut_points.push_back(cut_point);
			cut_normals.push_back(plane5.normal);
		}
	}
	if (distance3 > 0) {
		cut_point = ray.origin + distance3 * ray.direction;
		if (cut_point.x < max_.x && cut_point.x > min_.x && cut_point.z < max_.z && cut_point.z > min_.z) {
			cut_points.push_back(cut_point);
			cut_normals.push_back(plane3.normal);
		}
	}
	if (distance6 > 0) {
		cut_point = ray.origin + distance6 * ray.direction;
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
			if (glm::length(cut_points.at(it) - ray.origin) < glm::length(closest_cut - ray.origin)) {
				closest_cut = cut_points.at(it);
				closest_normal = cut_normals.at(it);
			}
		}
		t = glm::length(closest_cut - ray.origin);
		cut_point = closest_cut;
		normal = closest_normal;
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



