#include <glm/vec3.hpp>
#include "shape.hpp"
#include "color.hpp"
#include "ray.hpp"

class Sphere : public Shape{
public:
	Sphere(glm::vec3 center, double radius,std::string name, Color color);
	~Sphere();
	double area() const override;
	double volume() const override;
	std::ostream& print(std::ostream& os) const override;
	bool intersect(Ray const&) const;
	glm::vec3 center() const;
	double radius() const;
private:
	glm::vec3 center_;
	double radius_;
};