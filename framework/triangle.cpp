#include <glm/vec3.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/intersect.hpp>
#include <string>
#include <vector>
#include <algorithm>

#include "shape.hpp"
#include "color.hpp"
#include "ray.hpp"
#include "plane.hpp"
#include "triangle.hpp"
#include "renderer.hpp"


Triangle::Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c, std::string name, std::shared_ptr<Material> material) :
	Shape{ name, material }, a_{ a }, b_{ b }, c_{c} 
	{
		calculateBoundingBox();
	};

Triangle::Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 normal ,std::string name, std::shared_ptr<Material> material) :
	Shape{ name, material }, a_{ a }, b_{ b }, c_{c}, normal_{normal}, cust_normal_{true} {
		calculateBoundingBox();
};

Triangle::~Triangle() {}

std::ostream& Triangle::print(std::ostream& os) const {
	return Shape::print(os) << "A: (" << a_.x << ", " << a_.y << ", " << a_.z << "), B: (" << b_.x << ", " << b_.y << ", " << b_.z << "), C: (" << c_.x << ", " << c_.y << ", " << c_.z << ")\n";
};

//M�ller�Trumbore ray-triangle intersection algorithm was used
bool Triangle::intersect(Ray const& ray, float& distance ,glm::vec3& cut_point, glm::vec3& normal, std::shared_ptr<Shape>& shape) const {
	Ray transformedRay = transformRay(world_transformation_inv_, ray);

	const float EPSILON = 0.0000001;
	glm::vec3 a_b, a_c, h, s, q;
	float a, f, u, v;
	a_b = b_ - a_;
	a_c = c_ - a_;
	h = glm::cross(transformedRay.direction,a_c);
	a = glm::dot(a_b,h);
	//the ray is parallel
	if (a > -EPSILON && a < EPSILON) {
		return false;
	}
	f = 1 / a;
	s = transformedRay.origin - a_;
	u = f * (glm::dot(s,h));
	if (u < 0.0 || u > 1.0) {
		return false;
	}
	q = glm::cross(s,a_b);
	v = f * glm::dot(transformedRay.direction,q);
	if (v < 0.0 || u + v > 1.0) {
		return false;
	}
	// At this stage we can compute t to find out where the intersection point is on the line.
	float t = f * glm::dot(a_c,q);
	if (t > EPSILON) // ray intersection
	{
		cut_point = transformedRay.origin + transformedRay.direction * t;
		if(cust_normal_){
			normal = glm::normalize(normal_);
		} else {
			normal = glm::normalize(glm::cross(a_b, a_c));
		}

		glm::vec4 transformed_cut = world_transformation_ * glm::vec4{ cut_point, 1 };
		glm::vec4 transformed_normal = glm::normalize(glm::transpose(world_transformation_inv_) * glm::vec4{ normal , 0 });

		cut_point = glm::vec3{ transformed_cut.x, transformed_cut.y, transformed_cut.z };
		normal = glm::vec3{ transformed_normal.x, transformed_normal.y, transformed_normal.z };
		distance = glm::length(cut_point - ray.origin);
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
	std::vector<glm::vec3> points;
	points.push_back(a_);
	points.push_back(b_);
	points.push_back(c_);

	std::sort(points.begin(), points.end(), [](glm::vec3 p1, glm::vec3 p2)
		->bool {return p1.x < p2.x; });
	float min_x = points.at(0).x;
	float max_x = points.at(2).x;

	std::sort(points.begin(), points.end(), [](glm::vec3 p1, glm::vec3 p2)
		->bool {return p1.y < p2.y; });
	float min_y = points.at(0).y;
	float max_y = points.at(2).y;

	std::sort(points.begin(), points.end(), [](glm::vec3 p1, glm::vec3 p2)
		->bool {return p1.z < p2.z; });
	float min_z = points.at(0).z;
	float max_z = points.at(2).z;

	glm::vec3 min_bbox = glm::vec3{ min_x, min_y, min_z };
	glm::vec3 max_bbox = glm::vec3{ max_x, max_y, max_z };

    boundingBox_= std::make_shared<BoundingBox>(min_bbox,max_bbox);
}
