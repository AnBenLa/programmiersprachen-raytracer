#ifndef CONE_HEADER
#define CONE_HEADER
#include <glm/vec3.hpp>
#include "boundingbox.hpp"
#include "shape.hpp"
#include "color.hpp"
#include "ray.hpp"

class Cone : public Shape {
public:
	Cone(glm::vec3 base, glm::vec3 peak , float radius, std::string name, std::shared_ptr<Material> material);
	~Cone();
	std::ostream& print(std::ostream& os) const override;
	bool intersect(Ray const&, float&,glm::vec3&, glm::vec3&, std::shared_ptr<Shape>) const override;
	float radius() const;
	glm::vec3 peak() const;
	glm::vec3 base() const;
	std::shared_ptr<BoundingBox> boundingBox()const override;
	
private:
	float radius_;
	glm::vec3 peak_;
	glm::vec3 base_;
	void calculateBoundingBox();
};
#endif