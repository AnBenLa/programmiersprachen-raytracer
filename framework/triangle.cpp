#include <glm/vec3.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/intersect.hpp>
#include <string>
#include <vector>

#include "shape.hpp"
#include "color.hpp"
#include "ray.hpp"
#include "plane.hpp"
#include "triangle.hpp"


Triangle::Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c, std::string name, std::shared_ptr<Material> material) :
	Shape{ name, material }, a_{ a }, b_{ b }, c_{c} 
	{
		calculateBoundingBox();
	};

Triangle::Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 normal ,std::string name, std::shared_ptr<Material> material) :
	Shape{ name, material }, a_{ a }, b_{ b }, c_{c}, normal_{normal}, cust_normal_{true} {};

Triangle::~Triangle() {}

std::ostream& Triangle::print(std::ostream& os) const {
	return Shape::print(os) << "A: (" << a_.x << ", " << a_.y << ", " << a_.z << "), B: (" << b_.x << ", " << b_.y << ", " << b_.z << "), C: (" << c_.x << ", " << c_.y << ", " << c_.z << ")\n";
};

//M�ller�Trumbore ray-triangle intersection algorithm was used
bool Triangle::intersect(Ray const& ray, float& distance ,glm::vec3& cut_point, glm::vec3& normal, std::shared_ptr<Shape> shape) const {
	const float EPSILON = 0.0000001;
	glm::vec3 a_b, a_c, h, s, q;
	float a, f, u, v;
	a_b = b_ - a_;
	a_c = c_ - a_;
	h = glm::cross(ray.direction,a_c);
	a = glm::dot(a_b,h);
	//the ray is parallel
	if (a > -EPSILON && a < EPSILON) {
		return nullptr;
	}
	f = 1 / a;
	s = ray.origin - a_;
	u = f * (glm::dot(s,h));
	if (u < 0.0 || u > 1.0) {
		return nullptr;
	}
	q = glm::cross(s,a_b);
	v = f * glm::dot(ray.direction,q);
	if (v < 0.0 || u + v > 1.0) {
		return nullptr;
	}
	// At this stage we can compute t to find out where the intersection point is on the line.
	float t = f * glm::dot(a_c,q);
	if (t > EPSILON) // ray intersection
	{
		cut_point = ray.origin + ray.direction * t;
		distance = glm::length(cut_point - ray.origin);
		if(cust_normal_){
			normal = glm::normalize(normal_);
		} else {
			normal = glm::normalize(glm::cross(a_b, a_c));
		}
		shape = std::make_shared<Triangle>(a_, b_, c_, name(), material());
		return true;
	}
	else // This means that there is a line intersection but not a ray intersection.
		return false;
};

glm::vec3 Triangle::point_a() const {
	return a_;
}

glm::vec3 Triangle::point_b() const {
	return b_;
}

glm::vec3 Triangle::point_c() const {
	return c_;
}

std::shared_ptr<BoundingBox>Triangle::boundingBox()const
{
	return boundingBox_;
}

void Triangle::calculateBoundingBox()
{
	glm::vec3 AC{c_-a_};
	glm::vec3 minBbox{a_};
    glm::vec3 maxBbox{b_+AC};

    boundingBox_= std::make_shared<BoundingBox>(minBbox,maxBbox);
}
