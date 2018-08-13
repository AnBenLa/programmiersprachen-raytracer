#ifndef LIGHT_STRUCT
#define LIGHT_STRUCT
#include <glm/vec3.hpp>
#include "color.hpp"
struct Light{
	Light(std::string name, glm::vec3 position, Color color, double brightness) : name_(name), position_(position), color_(color), brightness_(brightness) {}

    std::string name_;
    glm::vec3 position_;
    Color color_;
    double brightness_;

	friend std::ostream& operator<<(std::ostream& os, Light const& l)
	{
		os << "Name: " << l.name_ << ", color: " << l.color_ << ", brightness: " << l.brightness_ << "\n";
		return os;
	}
};
#endif