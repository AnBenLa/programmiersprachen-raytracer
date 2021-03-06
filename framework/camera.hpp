#ifndef CAMERA_STRUCT
#define CAMERA_STRUCT
#include <glm/vec3.hpp>
struct Camera{
    std::string name_;
    glm::vec3 position_;
    glm::vec3 direction_;
    unsigned int fov_;
	glm::vec3 up_;
	glm::mat4x4 transformation_;

	Camera(std::string name, glm::vec3 pos, glm::vec3 dir, glm::vec3 up, unsigned int fov) : name_(name), position_(pos), direction_(dir), fov_(fov), up_(up) {
		transformation_ = glm::mat4x4{ 
			glm::vec4{1.0f,0.0f,0.0f,0.0f},
			glm::vec4{0.0f,1.0f,0.0f,0.0f},
			glm::vec4{0.0f,0.0f,1.0f,0.0f},
			glm::vec4{0.0f,0.0f,0.0f,1.0f}};
	}

	friend std::ostream& operator<<(std::ostream& os, Camera const& c)
	{
		os << "Name: " << c.name_ 
			<< ", Position: (" << c.position_.x << ", " << c.position_.y << ", " << c.position_.z 
			<< "), Direction: (" << c.direction_.x << ", " << c.direction_.y << ", " << c.direction_.z 
			<< "), Up: (" << c.up_.x << ", " << c.up_.y << ", " << c.up_.z 
			<< "), Fov: " << c.fov_ << "\n";
		return os;
	}
};
#endif