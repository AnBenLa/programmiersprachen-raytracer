#define _USE_MATH_DEFINES
#include <math.h>
#include <glm/vec3.hpp>
#include <glm/gtx/norm.hpp>    
#include <glm/gtx/intersect.hpp>
#include <string>

#include "shape.hpp"
#include "color.hpp"
#include "ray.hpp"
#include "cylinder.hpp"
#include "renderer.hpp"

Cylinder::Cylinder(glm::vec3 base, float height,float radius, std::string const& name, std::shared_ptr<Material> const& material):
    Shape{name, material},
	base_{base},
    height_{height},
    radius_{radius}
    {
		//transforms the cone so that its alligned with the y axis
		apply_transformation(glm::vec3{ 0 + base.x,  height + base.y, 0 + base.z }, -M_PI / 2, x_axis, glm::vec3{ radius,radius,height });
        calculateBoundingBox();
    };

Cylinder::~Cylinder(){};

std::ostream& Cylinder::print(std::ostream& os) const {
	return Shape::print(os)<<"Height: "<<height_<<", "<<"Radius: "<<radius_<<"\n";
}

bool Cylinder::intersect(Ray const& ray, float& distance ,glm::vec3& cut_point, glm::vec3& normal, std::shared_ptr<Shape>& shape)const
{	
	Ray transformedRay = transformRay(world_transformation_inv_, ray);

	float t = 0.0f;
	float a, b, c;
	a = pow(transformedRay.direction.x, 2) + pow(transformedRay.direction.y, 2);
	b = 2 * transformedRay.origin.x*transformedRay.direction.x + 2 * transformedRay.origin.y*transformedRay.direction.y;
	c = pow(transformedRay.origin.x, 2) + pow(transformedRay.origin.y, 2) - 1;
	float t_1, t_2;
	t_1 = (-b + sqrt(pow(b, 2) - 4 * a*c)) / (2 * a);
	t_2 = (-b - sqrt(pow(b, 2) - 4 * a*c)) / (2 * a);

	if (pow(b, 2) - 4 * a*c > 0) {
		bool c1 = false;
		bool c2 = false;
		glm::vec3 cut_1, cut_2;
		if (t_1 > 0) {
			cut_1 = transformedRay.origin + t_1 * transformedRay.direction;
			if (cut_1.z <= 0.0f && cut_1.z >= -1.0f) {
				c1 = true;
			}
		}
		if (t_2 > 0) {
			cut_2 = transformedRay.origin + t_2 * transformedRay.direction;
			if (cut_2.z <= 0.0f && cut_2.z >= -1.0f) {
				c2 = true;
			}
		}
		shape = std::make_shared<Cylinder>(base_, height_, radius_, name(), material());
		bool cut = false;
		glm::vec3 proj_z;
		if (c1 && c2) {
			if (t_2 < t_1) {
				cut_point = cut_2;
				distance = t_2;
				normal = glm::normalize(glm::vec3{ cut_point.x, cut_point.y, 0.0f });
				cut = true;
			}
			else {
				cut_point = cut_1;
				distance = t_1;
				normal = glm::normalize(glm::vec3{ cut_point.x, cut_point.y, 0.0f });
				cut = true;
			}
		}
		else if (c1) {
			cut_point = cut_1;
			distance = t_1;
			normal = glm::normalize(glm::vec3{ cut_point.x, cut_point.y, 0.0f });
			cut = true;
		}
		else if (c2) {
			cut_point = cut_2;
			distance = t_2;
			normal = glm::normalize(glm::vec3{ cut_point.x, cut_point.y, 0.0f });
			cut = true;
		}
		Plane plane_1{ glm::vec3{ 0,0,-1 }, glm::vec3{ 0,0,1 } };
		Plane plane_2{ glm::vec3{ 0,0,0 },	glm::vec3{ 0,0,1 } };
		float distance_base_1 = (glm::dot(plane_1.normal, plane_1.origin) - glm::dot(transformedRay.origin, plane_1.normal)) / (glm::dot(transformedRay.direction, plane_1.normal));
		float distance_base_2 = (glm::dot(plane_2.normal, plane_2.origin) - glm::dot(transformedRay.origin, plane_2.normal)) / (glm::dot(transformedRay.direction, plane_2.normal));
		float distance_base = distance_base_1;
		if (distance_base_2 > 0 && distance_base_2 < distance_base) {
			distance_base = distance_base_2;
		}
		if (distance_base > 0) {
			glm::vec3  base_cut = transformedRay.origin + distance_base * transformedRay.direction;
			if (glm::length(glm::vec3{ base_cut.x, base_cut.y, 0 }) <= 1) {
				if ((c1 && c2 && distance_base < t_1 && distance_base < t_2) || (c1 && distance_base < t_1) || (c2 && distance_base < t_2)) {
					cut_point = base_cut;
					distance = distance_base;
					normal = glm::normalize(glm::vec3{ cut_point.x, cut_point.y, 0.0f });
					cut = true;
				}
			}
		}
		if (cut) {

			glm::vec4 transformed_cut = world_transformation_ * glm::vec4{ cut_point, 1 };
			glm::vec4 transformed_normal = glm::normalize(glm::transpose(world_transformation_inv_) * glm::vec4{ normal , 0 });

			cut_point = glm::vec3{ transformed_cut.x, transformed_cut.y, transformed_cut.z };
			normal = glm::normalize(glm::vec3{ transformed_normal.x, transformed_normal.y, transformed_normal.z });
			distance = glm::length(cut_point - ray.origin);
			return true;
		}
	}
	return false;
}

void Cylinder::calculateBoundingBox()
{
    glm::vec3 minBbox{-1, -1 , -1};
    glm::vec3 maxBbox{1, 1, 0};

    boundingBox_= std::make_shared<BoundingBox>(minBbox,maxBbox);
}
