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
		apply_transformation(glm::vec3{ 0,0,0 }, 0, x_axis, glm::vec3{ radius,radius,height});
		apply_transformation(glm::vec3{ 0, 0 , 0 }, -M_PI/2, x_axis, glm::vec3{ 1,1,1 });
		apply_transformation(glm::vec3{ 0, 0 , 1 }, 0, x_axis, glm::vec3{ 1,1,1 });
		calculateBoundingBox();
	};

Cone::~Cone() {}

std::ostream& Cone::print(std::ostream& os) const {
	return Shape::print(os) << "Base: (" << base_.x << ", " << base_.y << ", " << base_.z << "), Height: " << height_ <<", Radius: "<< radius_ <<"\n";
};

bool Cone::intersect(Ray const& ray, float& distance ,glm::vec3& cut_point, glm::vec3& normal, std::shared_ptr<Shape>& shape) const {
	/*
	glm::vec3 v = ray.direction;
	glm::vec3 h = base_ - peak_;
	glm::vec3 h_norm = glm::normalize(h);
	glm::vec3 w = ray.origin - peak_;
	float m = (pow(radius_,2)) / pow(glm::length(h),2);
	float v_h = glm::dot(ray.direction, h_norm);
	float w_h = glm::dot(w, h_norm);

	float a = glm::dot(v,v) - m*pow(v_h,2) - pow(v_h, 2);
	float b = 2*(glm::dot(v,w) - m*v_h*w_h - v_h*w_h);
	float c = glm::dot(w,w) - m*pow(w_h,2) - pow(w_h,2);

	if (pow(b, 2) - 4 * a*c >= 0) {
		float t_1 = (-b + sqrt(pow(b, 2) - 4 * a*c)) / (2 * a);
		float t_2 = (-b - sqrt(pow(b, 2) - 4 * a*c)) / (2 * a);
		if (t_2 < t_1 && t_2 > 0) {
			t_1 = t_2;
		}
		if (pow(b, 2) - 4 * a*c == 0) {
			t_1 = -b / (2 * a);
		}
		
		cut_point = ray.origin + t_1 * glm::normalize(ray.direction);
		
		if (0 < glm::dot(cut_point - peak_, h) && glm::length(peak_-cut_point) <= sqrt(pow(radius_,2) + pow(glm::length(h),2))) {
			//normal not correct yet
			float r = sqrt(pow(cut_point.x - base_.x, 2) + pow(cut_point.z - base_.z, 2));
			float z = r * (radius_ / glm::length(h));
			glm::vec3 p = glm::vec3{ cut_point.x - base_.x, z, cut_point.z - base_.z };
			normal = glm::normalize(p);
			distance = glm::length(cut_point - ray.origin);
			shape = std::make_shared<Cone>(base_, peak_, radius_, name(), material());
			return true;
		}
		return false;

	} 
	*/

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
		if (c1 && c2) {
			if (t_2 < t_1) {
				cut_point = cut_2;
				distance = t_2;
				glm::vec3 proj_z = glm::normalize(glm::vec3{ cut_point.x, cut_point.y, 0 });
				normal = glm::normalize(glm::vec3{ proj_z.x, proj_z.y , 1.0f });

				glm::vec4 transformed_cut = world_transformation_ * glm::vec4{ cut_2, 1 };
				glm::vec4 transformed_normal = glm::normalize(world_transformation_ * glm::vec4{ normal , 0 });
				cut_point = glm::vec3{ transformed_cut.x, transformed_cut.y, transformed_cut.z };
				normal = glm::vec3{ transformed_normal.x, transformed_normal.y, transformed_normal.z };
				distance = glm::length(cut_point - ray.origin);
				return true;
			}
			else {
				cut_point = cut_1;
				distance = t_1;
				glm::vec3 proj_z = glm::normalize(glm::vec3{ cut_point.x, cut_point.y, 0 });
				normal = glm::normalize(glm::vec3{ proj_z.x, proj_z.y , 1.0f });

				glm::vec4 transformed_cut = world_transformation_ * glm::vec4{ cut_2, 1 };
				glm::vec4 transformed_normal = glm::normalize(world_transformation_ * glm::vec4{ normal , 0 });
				cut_point = glm::vec3{ transformed_cut.x, transformed_cut.y, transformed_cut.z };
				normal = glm::vec3{ transformed_normal.x, transformed_normal.y, transformed_normal.z };
				distance = glm::length(cut_point - ray.origin);
				return true;
			}
		}
		else if (c1) {
			cut_point = cut_1;
			distance = t_1;
			glm::vec3 proj_z = glm::normalize(glm::vec3{ cut_point.x, cut_point.y, 0 });
			normal = glm::normalize(glm::vec3{ proj_z.x, proj_z.y , 1.0f });

			glm::vec4 transformed_cut = world_transformation_ * glm::vec4{ cut_2, 1 };
			glm::vec4 transformed_normal = glm::normalize(world_transformation_ * glm::vec4{ normal , 0 });
			cut_point = glm::vec3{ transformed_cut.x, transformed_cut.y, transformed_cut.z };
			normal = glm::vec3{ transformed_normal.x, transformed_normal.y, transformed_normal.z };
			distance = glm::length(cut_point - ray.origin);
			return true;
		}
		else if (c2) {
			cut_point = cut_2;
			distance = t_2;
			glm::vec3 proj_z = glm::normalize(glm::vec3{ cut_point.x, cut_point.y, 0 });
			normal = glm::normalize(glm::vec3{ proj_z.x, proj_z.y , 1.0f });

			glm::vec4 transformed_cut = world_transformation_ * glm::vec4{ cut_2, 1 };
			glm::vec4 transformed_normal = glm::normalize(world_transformation_ * glm::vec4{ normal , 0 });
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



