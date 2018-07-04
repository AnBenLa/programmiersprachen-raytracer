#include "box.hpp"
#include <glm/vec3.hpp>
#include <glm/gtx/norm.hpp>
#include "shape.hpp"
#include "color.hpp"
#include "ray.hpp"
#include <string>

Box::Box(glm::vec3 min, glm::vec3 max, std::string name, std::shared_ptr<Material> material) :
	Shape{ name, material }, min_{ min }, max_{ max } { std::cout << "Box constructor\n"; };

Box::~Box() { std::cout << "Base destructor\n"; }

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
	return Shape::print(os) << "Shape form: Box\n";
};

//Part of this method could be put in a helpher method, this would reduce code
bool Box::intersect(Ray const& ray, float& t) const{

	auto origin = ray.origin;
	auto direction = ray.direction;
	bool intersect = false;
	float smallest_pos = -1;

	//Checks for the intersection point of the direction and the x-axis if it is inbetween the box borders
	if (direction.x != 0) {
		//calculates the factor by which the direction has to be multiplied to cut the yz-plane at x-min and x-max
		auto fkt = (min_.x - origin.x) / direction.x;
		auto fkt2 = (max_.x - origin.x) / direction.x;
		//calculates the points where the direction multiplied with that factor hits the plane
		auto point = origin + fkt * direction;
		auto point2 = origin + fkt2 * direction;
		//checks if the resulting points are within the box borders (inbetween the y and z values)
		auto s1 = (point.y < max_.y && point.y > min_.y && point.z < max_.z && point.z > min_.z);
		auto s2 = (point2.y < max_.y && point2.y > min_.y && point2.z < max_.z && point2.z > min_.z);
		//calculates the distance between the intersection points and the ray origin
		auto dist_point = glm::distance(point, origin) * (fkt / abs(fkt));
		auto dist_point2 = glm::distance(point2, origin) * (fkt2 / abs(fkt2));
		//if the ray cuts both zy-planes at x-min and x-max or only one set t to the minimal positiv distance
		//if no smaller positiv distance is set yet
		if (s1 && s2) {
			if (dist_point < dist_point2) {
				if (dist_point > 0 && (dist_point < smallest_pos || smallest_pos < 0)) {
					smallest_pos = dist_point;
				}
				else if (dist_point2 > 0 && (dist_point2 < smallest_pos || smallest_pos < 0)) {
					smallest_pos = dist_point2;
				}
			}
			else {
				if (dist_point2 > 0 && (dist_point2 < smallest_pos || smallest_pos < 0)) {
					smallest_pos = dist_point2;
				}
				else if (dist_point > 0 && (dist_point < smallest_pos || smallest_pos < 0)) {
					smallest_pos = dist_point;
				}
			}
			intersect = true;
		}
		else if (s1) {
			if (dist_point > 0 && (dist_point < smallest_pos || smallest_pos < 0)) {
				smallest_pos = dist_point;
			}
			intersect = true;
		}
		else if (s2) {
			if (dist_point2 > 0 && (dist_point2 < smallest_pos || smallest_pos < 0)) {
				smallest_pos = dist_point2;
			}
			intersect = true;
		}
	}
	//Checks for the intersection point of the direction and the y-axis if it is inbetween the box borders
	if (direction.y != 0) {
		//calculates the factor by which the direction has to be multiplied to cut the xz-plane at y-min and y-max
		auto fkt = (min_.y - origin.y) / direction.y;
		auto fkt2 = (max_.y - origin.y) / direction.y;
		//calculates the points where the direction multiplied with that factor hits the plane
		auto point = origin + fkt * direction;
		auto point2 = origin + fkt2 * direction;
		//checks if the resulting points are within the box borders (inbetween the x and z values)
		auto s1 = (point.x < max_.x && point.x > min_.x && point.z < max_.z && point.z > min_.z);
		auto s2 = (point2.x < max_.x && point2.x > min_.y && point2.z < max_.z && point2.z > min_.z);
		//calculates the distance between the intersection points and the ray origin
		auto dist_point = glm::distance(point, origin) * (fkt / abs(fkt));
		auto dist_point2 = glm::distance(point2, origin) * (fkt2 / abs(fkt2));
		//if the ray cuts both xz-planes at y-min and y-max  or only one set t to the minimal positiv distance
		//if no smaller positiv distance is set yet
		if (s1 && s2) {
			if (dist_point < dist_point2) {
				if (dist_point > 0 && (dist_point < smallest_pos || smallest_pos < 0)) {
					smallest_pos = dist_point;
				}
				else if (dist_point2 > 0 && (dist_point2 < smallest_pos || smallest_pos < 0)) {
					smallest_pos = dist_point2;
				}
			}
			else {
				if (dist_point2 > 0 && (dist_point2 < smallest_pos || smallest_pos < 0)) {
					smallest_pos = dist_point2;
				}
				else if (dist_point > 0 && (dist_point < smallest_pos || smallest_pos < 0)) {
					smallest_pos = dist_point;
				}
			}
			intersect = true;
		}
		else if (s1) {
			if (dist_point > 0 && (dist_point < smallest_pos || smallest_pos < 0)) {
				smallest_pos = dist_point;
			}
			intersect = true;
		}
		else if (s2) {
			if (dist_point2 > 0 && (dist_point2 < smallest_pos || smallest_pos < 0)) {
				smallest_pos = dist_point2;
			}
			intersect = true;
		}
	}
	//Checks for the intersection point of the direction and the z-axis if it is inbetween the box borders
	if (direction.z != 0) {
		//calculates the factor by which the direction has to be multiplied to cut the yx-plane at z-min and z-max
		auto fkt = (min_.z - origin.z) / direction.z;
		auto fkt2 = (max_.z - origin.z) / direction.z;
		//calculates the points where the direction multiplied with that factor hits the plane
		auto point = origin + fkt * direction;
		auto point2 = origin + fkt2 * direction;
		//checks if the resulting points are within the box borders (inbetween the x and y values)
		auto s1 = (point.y < max_.y && point.y > min_.y && point.x < max_.x && point.x > min_.x);
		auto s2 = (point2.y < max_.y && point2.y > min_.y && point2.x < max_.x && point2.x > min_.x);
		//calculates the distance between the intersection points and the ray origin
		auto dist_point = glm::distance(point, origin) * (fkt / abs(fkt));
		auto dist_point2 = glm::distance(point2, origin) * (fkt2 / abs(fkt2));
		//if the ray cuts both yx-planes at z-min and z-max  or only one set t to the minimal positiv distance
		//if no smaller positiv distance is set yet
		if (s1 && s2) {
			if (dist_point < dist_point2) {
				if (dist_point > 0 && (dist_point < smallest_pos || smallest_pos < 0)) {
					smallest_pos = dist_point;
				}
				else if (dist_point2 > 0 && (dist_point2 < smallest_pos || smallest_pos < 0)) {
					smallest_pos = dist_point2;
				}
			}
			else {
				if (dist_point2 > 0 && (dist_point2 < smallest_pos || smallest_pos < 0)) {
					smallest_pos = dist_point2;
				}
				else if (dist_point > 0 && (dist_point < smallest_pos || smallest_pos < 0)) {
					smallest_pos = dist_point;
				}
			}
			intersect = true;
		}
		else if (s1) {
			if (dist_point > 0 && (dist_point < smallest_pos || smallest_pos < 0)) {
				smallest_pos = dist_point;
			}
			intersect = true;
		}
		else if (s2) {
			if (dist_point2 > 0 && (dist_point2 < smallest_pos || smallest_pos < 0)) {
				smallest_pos = dist_point2;
			}
			intersect = true;
		}
	}
	t = smallest_pos;

	return intersect;
};

glm::vec3 Box::min() const {
	return min_;
}

glm::vec3 Box::max() const {
	return max_;
}



