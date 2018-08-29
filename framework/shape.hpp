#ifndef SHAPE_HEADER
#define SHAPE_HEADER
#include <string>
#include <glm/glm.hpp>
#include <memory>
#include <iostream>
#include <tuple>

#include "color.hpp"
#include "ray.hpp"
#include "material.hpp"
#include "light.hpp"
#include "boundingbox.hpp"

class Shape : std::enable_shared_from_this<Shape> {
public:
	Shape(std::string const& name, std::shared_ptr<Material> const& material);
	~Shape();
	virtual std::ostream& print(std::ostream& os) const;
	virtual bool intersect(Ray const& incoming_ray, float& distance, glm::vec3& cut_point, glm::vec3& normal_vec, std::shared_ptr<Shape>& shape) const = 0;
	virtual std::shared_ptr<BoundingBox> boundingBox()const = 0;
	std::string name() const;
	std::shared_ptr<Material> material() const;

private:
	std::string name_;
	std::shared_ptr<Material> material_;
	glm::mat4 world_transformation_;
	glm::mat4 world_transformation_inv_;

protected:
	std::shared_ptr<BoundingBox> boundingBox_;

};

std::ostream& operator <<(std::ostream& os, Shape const& s);
#endif