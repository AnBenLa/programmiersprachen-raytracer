#ifndef PLANE_STRUCT
#define PLANE_STRUCT
#include <glm/vec3.hpp>
struct Plane
{
	glm::vec3 origin;
	glm::vec3 direction1;
	glm::vec3 direction2;
};
#endif