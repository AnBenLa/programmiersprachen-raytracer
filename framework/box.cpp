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
	
	Plane plane1{ min_, glm::vec3 { 1,0,0 } };
	Plane plane2{ min_, glm::vec3 { 0,0,1 } };
	Plane plane3{ min_, glm::vec3 { 0,1,0 } };
	Plane plane4{ max_, glm::vec3 { 1,0,0 } };
	Plane plane5{ max_, glm::vec3 { 0,0,1 } };
	Plane plane6{ max_, glm::vec3 { 0,1,0 } };

	//distance positiv oder negativ?
	float distance1, distance2, distance3, distance4, distance5, distance6;

	bool inter1 = glm::intersectRayPlane(ray.origin, ray.direction, plane1.origin, plane1.normal, distance1);
	bool inter2 = glm::intersectRayPlane(ray.origin, ray.direction, plane2.origin, plane2.normal, distance2);
	bool inter3 = glm::intersectRayPlane(ray.origin, ray.direction, plane3.origin, plane3.normal, distance3);
	bool inter4 = glm::intersectRayPlane(ray.origin, ray.direction, plane4.origin, plane4.normal, distance4);
	bool inter5 = glm::intersectRayPlane(ray.origin, ray.direction, plane5.origin, plane5.normal, distance5);
	bool inter6 = glm::intersectRayPlane(ray.origin, ray.direction, plane6.origin, plane6.normal, distance6);

	std::vector<glm::vec3> cut_points;
	std::vector<glm::vec3> cut_normals;

	if (inter1 && distance1 > 0) {
		cut_point = ray.origin + distance1 * ray.direction;
		if (cut_point.y < max_.y && cut_point.y > min_.y && cut_point.z < max_.z && cut_point.z > min_.z) {
			cut_points.push_back(cut_point);
			cut_normals.push_back(plane1.normal);
		}
	}
	if (inter4 && distance4 > 0) {
		cut_point = ray.origin + distance4 * ray.direction;
		if (cut_point.y < max_.y && cut_point.y > min_.y && cut_point.z < max_.z && cut_point.z > min_.z) {
			cut_points.push_back(cut_point);
			cut_normals.push_back(plane4.normal);
		}
	}
	if (inter2  && distance2 > 0){
		cut_point = ray.origin + distance2 * ray.direction;
		if (cut_point.y < max_.y && cut_point.y > min_.y && cut_point.x < max_.x && cut_point.x > min_.x) {
			cut_points.push_back(cut_point);
			cut_normals.push_back(plane2.normal);
		}
	}
	if (inter5 && distance5 > 0) {
		cut_point = ray.origin + distance5 * ray.direction;
		if (cut_point.y < max_.y && cut_point.y > min_.y && cut_point.x < max_.x && cut_point.x > min_.x) {
			cut_points.push_back(cut_point);
			cut_normals.push_back(plane5.normal);
		}
	}
	if (inter3 && distance3 > 0) {
		cut_point = ray.origin + distance3 * ray.direction;
		if (cut_point.x < max_.x && cut_point.x > min_.x && cut_point.z < max_.z && cut_point.z > min_.z) {
			cut_points.push_back(cut_point);
			cut_normals.push_back(plane3.normal);
		}
	}
	if (inter6 && distance6 > 0) {
		cut_point = ray.origin + distance6 * ray.direction;
		if (cut_point.x < max_.x && cut_point.x > min_.x && cut_point.z < max_.z && cut_point.z > min_.z) {
			cut_points.push_back(cut_point);
			cut_normals.push_back(plane6.normal);
		}
	}

	//behind or in front of camera!

	if (cut_points.size() > 0) {
		glm::vec3 closest_cut = cut_points.at(0);
		glm::vec3 closest_normal = cut_normals.at(0);

		for (auto it = 0; it < cut_points.size(); ++it) {
			if (glm::length(cut_points.at(it)) < glm::length(closest_cut)) {
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

Ray Box::reflect(glm::vec3 cut_point, Ray const& incoming_ray) const {
	Ray outcoming_ray{};
	return outcoming_ray;
}

glm::vec3 Box::min() const {
	return min_;
}

glm::vec3 Box::max() const {
	return max_;
}



