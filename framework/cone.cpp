#include <glm/vec3.hpp>
#include <glm/gtx/norm.hpp>
#include <string>
#include <algorithm>

#include "shape.hpp"
#include "color.hpp"
#include "ray.hpp"
#include "cone.hpp"
#include "box.hpp"
#include "renderer.hpp"


Cone::Cone(glm::vec3 base, float height, float radius ,std::string name, std::shared_ptr<Material> material) :
	Shape{ name, material }, base_{ base }, height_{ height }, radius_{radius} 
	{
		//transforms the cone so that it stands on the y axis with the base at the origin
		apply_transformation(glm::vec3{ 0, height, 0 }, -M_PI / 2, x_axis, glm::vec3{ radius,radius,height });
		calculateBoundingBox();
	};

Cone::~Cone() {}

std::ostream& Cone::print(std::ostream& os) const {
	return Shape::print(os) << "Base: (" << base_.x << ", " << base_.y << ", " << base_.z << "), Height: " << height_ <<", Radius: "<< radius_ <<"\n";
};

bool Cone::intersect(Ray const& ray, float& distance ,glm::vec3& cut_point, glm::vec3& normal, std::shared_ptr<Shape>& shape) const {
	Ray transformedRay = transformRay(world_transformation_inv_, ray);

	float t = 0.0f;
	float a, b, c;
	a = pow(transformedRay.direction.x, 2) + pow(transformedRay.direction.y, 2) - pow(transformedRay.direction.z, 2);
	b = 2 * transformedRay.origin.x*transformedRay.direction.x + 2 * transformedRay.origin.y*transformedRay.direction.y - 2 * transformedRay.origin.z*transformedRay.direction.z;
	c = pow(transformedRay.origin.x, 2) + pow(transformedRay.origin.y, 2) - pow(transformedRay.origin.z, 2);
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
		shape = std::make_shared<Cone>(base_, height_,radius_,name(), material());
		bool cut = false;
		glm::vec3 proj_z;
		if (c1 && c2) {
			if (t_2 < t_1) {
				cut_point = cut_2;
				distance = t_2;
				proj_z = glm::normalize(glm::vec3{ cut_point.x, cut_point.y, 0 });
				cut =  true;
			}
			else {
				cut_point = cut_1;
				distance = t_1;
				proj_z = glm::normalize(glm::vec3{ cut_point.x, cut_point.y, 0 });
				cut = true;
			}
		}
		else if (c1) {
			cut_point = cut_1;
			distance = t_1;
			proj_z = glm::normalize(glm::vec3{ cut_point.x, cut_point.y, 0 });
			cut = true;
		}
		else if (c2) {
			cut_point = cut_2;
			distance = t_2;
			proj_z = glm::normalize(glm::vec3{ cut_point.x, cut_point.y, 0 });
			cut = true;
		}
		if (cut) {
			normal = glm::normalize(glm::vec3{ proj_z.x, proj_z.y , 1.0f });
			glm::vec4 transformed_cut = world_transformation_ * glm::vec4{ cut_point, 1 };
			glm::vec4 transformed_normal = glm::normalize(glm::transpose(world_transformation_inv_) * glm::vec4{ normal , 0 });
			
			cut_point = glm::vec3{ transformed_cut.x, transformed_cut.y, transformed_cut.z };
			normal = glm::vec3{ transformed_normal.x, transformed_normal.y, transformed_normal.z };
			distance = glm::length(cut_point - ray.origin);
			return true;
		}
	}
	return false;
};

float Cone::height() const {
	return height_;
}

glm::vec3 Cone::base() const {
	return base_;
}

float Cone::radius() const {
	return radius_;
}

void Cone::calculateBoundingBox()
{
	glm::vec3 minBbox{base_.x-radius_,base_.y-radius_,base_.z - height_};
    glm::vec3 maxBbox{base_.x+radius_,base_.y+radius_,base_.z+radius_};

    boundingBox_= std::make_shared<BoundingBox>(minBbox,maxBbox);
}

std::shared_ptr<BoundingBox> Cone::boundingBox() const{
	return boundingBox_;
}



