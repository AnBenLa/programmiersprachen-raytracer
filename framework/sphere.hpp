#include <glm/vec3.hpp>
#include "shape.hpp"

class Sphere : public Shape{
public:
	Sphere(glm::vec3 center, double radius);
	double area() const override;
	double volume() const override;
	glm::vec3 center() const;
	double radius() const;
private:
	glm::vec3 center_;
	double radius_;
};