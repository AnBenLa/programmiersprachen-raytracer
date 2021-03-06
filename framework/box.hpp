#ifndef BOX_HEADER
#define BOX_HEADER
#include <glm/vec3.hpp>
#include "shape.hpp"
#include "color.hpp"
#include "ray.hpp"

class Box : public Shape {

public:
	Box(glm::vec3 min, glm::vec3 max, std::string name, std::shared_ptr<Material> material);
	~Box();
	std::ostream& print(std::ostream& os) const override;
	bool intersect(Ray const&, float&,glm::vec3&, glm::vec3&, std::shared_ptr<Shape>&) const override;
	glm::vec3 min() const;
	glm::vec3 max() const;

private:
	glm::vec3 min_;
	glm::vec3 max_;
};
#endif