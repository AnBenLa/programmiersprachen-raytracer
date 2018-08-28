#ifndef BOUNDING_BOX_STRUCT
#define BOUNDING_BOX_STRUCT
#include <glm/vec3.hpp>
#include "plane.hpp"
#include "ray.hpp"

struct BoundingBox {
	BoundingBox(glm::vec3 min, glm::vec3 max) : min_(min), max_(max) {}

	glm::vec3 min_;
	glm::vec3 max_;

	bool intersect(Ray const& ray) const {
		glm::vec3 cut_point;

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

		//Checking for each plane if it is in front of the camera
		if (distance1 > 0) {
			cut_point = ray.origin + distance1 * ray.direction;
			if (cut_point.y < max_.y && cut_point.y > min_.y && cut_point.z < max_.z && cut_point.z > min_.z) {
				return true;
			}
		}
		if (distance4 > 0) {
			cut_point = ray.origin + distance4 * ray.direction;
			if (cut_point.y < max_.y && cut_point.y > min_.y && cut_point.z < max_.z && cut_point.z > min_.z) {
				return true;
			}
		}
		//untere Fl�che
		if (distance2 > 0) {
			cut_point = ray.origin + distance2 * ray.direction;
			if (cut_point.y < max_.y && cut_point.y > min_.y && cut_point.x < max_.x && cut_point.x > min_.x) {
				return true;
			}
		}
		//obere Fl�che
		if (distance5 > 0) {
			cut_point = ray.origin + distance5 * ray.direction;
			if (cut_point.y < max_.y && cut_point.y > min_.y && cut_point.x < max_.x && cut_point.x > min_.x) {
				return true;
			}
		}
		if (distance3 > 0) {
			cut_point = ray.origin + distance3 * ray.direction;
			if (cut_point.x < max_.x && cut_point.x > min_.x && cut_point.z < max_.z && cut_point.z > min_.z) {
				return true;
			}
		}
		if (distance6 > 0) {
			cut_point = ray.origin + distance6 * ray.direction;
			if (cut_point.x < max_.x && cut_point.x > min_.x && cut_point.z < max_.z && cut_point.z > min_.z) {
				return true;
			}
		}
		return false;
	};
};

#endif