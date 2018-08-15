// -----------------------------------------------------------------------------
// Copyright  : (C) 2014-2017 Andreas-C. Bernstein
// License    : MIT (see the file LICENSE)
// Maintainer : Andreas-C. Bernstein <andreas.bernstein@uni-weimar.de>
// Stability  : experimental
//
// Renderer
// -----------------------------------------------------------------------------

#define _USE_MATH_DEFINES
#include "renderer.hpp"
#include <math.h>

Renderer::Renderer(unsigned w, unsigned h, std::string const& file)
  : width_(w)
  , height_(h)
  , color_buffer_(w*h, Color(0.0, 0.0, 0.0))
  , filename_(file)
  , ppm_(width_, height_)
{}

void Renderer::render(Scene const& scene)
{
	double d = (width_/2)/tan(scene.camera_->fov_/2 * M_PI / 180);
	
	for (unsigned y = 0; y < height_; ++y) {
		for (unsigned x = 0; x < width_; ++x) {
			Pixel p(x,y);
			p.color = Color(0.0, 0.0, float(y) / width_);
			float min_distance = 0.0f;
			std::shared_ptr<Shape> closest_shape = nullptr;
			glm::vec3 closest_cut{};
			glm::vec3 closest_normal{};

			for (std::shared_ptr<Shape> shape_ptr : scene.shape_vec_) {
				float distance = 0.0f;
				glm::vec3 cut_point{};
				glm::vec3 normal{};
				glm::vec3 pos = scene.camera_->position_;
				glm::vec3 dir = scene.camera_->direction_;

				//TODO needs to be changed because something with the direction is off
				dir = dir + glm::vec3{ x - (0.5 * width_),y - (0.5 * height_),-d };
				Ray ray{ pos , glm::normalize(dir) };
				
				if ((*shape_ptr).intersect(ray, distance, cut_point, normal)) {
					if (distance < min_distance || (min_distance == 0.0f && closest_shape == nullptr)) {
						min_distance = distance;
						closest_shape = shape_ptr;
						closest_cut = cut_point;
						closest_normal = normal;
					}
				}
			}
			if (closest_shape != nullptr) {
				p.color = calculate_color(closest_shape, closest_cut, closest_normal, scene);
			}
			write(p);
		}
	}
	ppm_.save(filename_);
}

Color Renderer::calculate_color(std::shared_ptr<Shape> shape, glm::vec3 const& cut, glm::vec3 const& normal, Scene const& scene) {
	Color color{ 0.0f, 0.0f, 0.0f };
	for (std::shared_ptr<Light> light : scene.light_vec_) {
		for (std::shared_ptr<Shape> shape_ptr : scene.shape_vec_) {
			float distance;
			glm::vec3 cut_point;
			glm::vec3 normal_new;
			glm::vec3 vec_to_light = light->position_ - cut;
			if (!shape_ptr->intersect(Ray{cut,vec_to_light}, distance, cut_point, normal_new)) {
				float o = cos(glm::dot(glm::normalize(normal), glm::normalize(vec_to_light)));
				float i_p = light->brightness_;
				Color k_d = shape->material()->kd;
				color += k_d*o*i_p;
			}
		}
	}
	return color;
}

void Renderer::write(Pixel const& p)
{
  // flip pixels, because of opengl glDrawPixels
  size_t buf_pos = (width_*p.y + p.x);
  if (buf_pos >= color_buffer_.size() || (int)buf_pos < 0) {
    std::cerr << "Fatal Error Renderer::write(Pixel p) : "
      << "pixel out of ppm_ : "
      << (int)p.x << "," << (int)p.y
      << std::endl;
  } else {
    color_buffer_[buf_pos] = p.color;
  }

  ppm_.write(p);
}
