#ifndef LIGHT_STRUCT
#define LIGHT_STRUCT
#include <glm/vec3.hpp>
#include "color.hpp"
struct Light{
    std::string name_;
    glm::vec3 position_;
    Color color_;
    double brightness_;
};
#endif