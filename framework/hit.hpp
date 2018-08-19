#ifndef HIT_POINT_STRUCT
#define HIT_POINT_STRUCT
#include <glm/vec3.hpp>
#include "shape.hpp"
struct Hit {
	Hit(glm::vec3 incoming_vec, glm::vec3 position, glm::vec3 normal, std::shared_ptr<Shape> shape) : incoming_vec_(incoming_vec), position_(position), normal_(normal), shape_(shape){}

	glm::vec3 incoming_vec_;
	glm::vec3 position_;
	glm::vec3 normal_;
	std::shared_ptr<Shape> shape_;
};
#endif