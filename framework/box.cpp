#include "box.hpp"
#include <glm/vec3.hpp>
#include "shape.hpp"
#include "color.hpp"

Box::Box(glm::vec3 min, glm::vec3 max, std::string name, Color color) : 
	Shape{ name, color }, min_{ min }, max_{ max } {};

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

glm::vec3 Box::min() const {
	return min_;
}

glm::vec3 Box::max() const {
	return max_;
}

