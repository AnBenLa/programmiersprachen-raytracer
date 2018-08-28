#ifndef CONE_HEADER
#define CONE_HEADER
#include <glm/vec3.hpp>
#include "shape.hpp"
#include "color.hpp"
#include "ray.hpp"

class Cone : public Shape {
public:
	Cone(glm::vec3 base, glm::vec3 peak , float radius, std::string name, std::shared_ptr<Material> material);
	~Cone();
	double area() const override;
	double volume() const override;
	std::ostream& print(std::ostream& os) const override;
	bool intersect(Ray const&, float& r, glm::vec3&, glm::vec3&) const override;
	float radius() const;
	glm::vec3 peak() const;
	glm::vec3 base() const;
	std::shared_ptr<Box>getBoundingBox()const override;
	
private:
	float radius_;
	glm::vec3 peak_;
	glm::vec3 base_;
	void calculateBoundingBox();
};
#endif