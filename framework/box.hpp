#include <glm/vec3.hpp>
#include "shape.hpp"
#include "color.hpp"
class Box : public Shape {
public:
	Box(glm::vec3 min, glm::vec3 max, std::string name, Color color);
	double area() const override;
	double volume() const override;
	glm::vec3 min() const;
	glm::vec3 max() const;
private:
	glm::vec3 min_;
	glm::vec3 max_;
};