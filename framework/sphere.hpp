#include <glm/vec3.hpp>
#include "shape.hpp"
#include "color.hpp"

class Sphere : public Shape{
public:
	Sphere(glm::vec3 center, double radius,std::string name, Color color);
	double area() const override;
	double volume() const override;
	glm::vec3 center() const;
	double radius() const;
private:
	glm::vec3 center_;
	double radius_;
};