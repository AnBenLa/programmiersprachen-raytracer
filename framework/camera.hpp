#ifndef CAMERA_STRUCT
#define CAMERA_STRUCT
#include <glm/vec3.hpp>
#include "color.hpp"
#include "ray.hpp"
struct Camera{
    std::string name_;
    glm::vec3 position_;
    Ray direction_;
    unsigned int fov_;
};
#endif