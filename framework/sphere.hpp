#ifndef SPHERE_HEADER
#define SPHERE_HEADER
#include <glm/vec3.hpp>
#include "shape.hpp"
#include "color.hpp"
#include "ray.hpp"

class Sphere : public Shape{
public:
	Sphere(glm::vec3 center, double radius,std::string name, std::shared_ptr<Material> material);
	~Sphere();
	std::ostream& print(std::ostream& os) const override;
	bool intersect(Ray const&, float&,glm::vec3&, glm::vec3&, std::shared_ptr<Shape>&) const override;
	glm::vec3 center() const;
	double radius() const;

private:
	glm::vec3 center_;
	double radius_;
	void calculateBoundingBox();
};
#endif