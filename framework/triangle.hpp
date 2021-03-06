#ifndef TRIANGLE_HEADER
#define TRIANGLE_HEADER
#include <glm/vec3.hpp>
#include "shape.hpp"
#include "color.hpp"
#include "ray.hpp"
#include "box.hpp"

class Triangle : public Shape {
public:
	Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c, std::string name, std::shared_ptr<Material> material);
	Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 normal, std::string name, std::shared_ptr<Material> material);
	~Triangle();
	std::ostream& print(std::ostream& os) const override;
	bool intersect(Ray const&, float& ,glm::vec3&, glm::vec3&, std::shared_ptr<Shape>&) const override;
	glm::vec3 point_a() const;
	glm::vec3 point_b() const;
	glm::vec3 point_c() const;
	
private:
	glm::vec3 a_;
	glm::vec3 b_;
	glm::vec3 c_;
	glm::vec3 normal_;
	bool cust_normal_ = false;

	void calculateBoundingBox();
};
#endif