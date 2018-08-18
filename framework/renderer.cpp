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
#include <glm/gtx/vector_angle.hpp>
#include <math.h>
#include <string>
#include <chrono>

Renderer::Renderer(unsigned w, unsigned h, std::string const& file)
  : width_(w)
  , height_(h)
  , color_buffer_(w*h, Color(0.0, 0.0, 0.0))
  , filename_(file)
  , ppm_(width_, height_)
{}

void Renderer::render(Scene const& scene, int frames)
{
	double d = (width_ / 2) / tan(scene.camera_->fov_ / 2 * M_PI / 180);
	double frame_times = 0;
	for (int i = 0; i < frames;++i) {
		//scene.camera_->position_ = glm::vec3{0, 0 ,-200 + i};
		scene.light_vec_.at(0)->position_ = glm::vec3{ 500,800,i*6 };
		auto start = std::chrono::high_resolution_clock::now();
	
		for (unsigned y = 0; y < height_; ++y) {
			for (unsigned x = 0; x < width_; ++x) {
				
				Pixel p(x, y);
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
					glm::vec3 dir = glm::normalize(scene.camera_->direction_);

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
					//p.color = calculate_depth_map(closest_cut, scene, 300);
					Color current_color = calculate_color(closest_shape, closest_cut, closest_normal, scene);
					//tone mapping ???
					p.color = current_color;
				}
				write(p);
			}
		}
		auto finish = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = finish - start;
		double elapsed_s = elapsed.count();
		std::cout << "Frame rendered: "<< ((float)i+1)/(float)frames*100 <<"% \t";
		std::cout << ", Elapsed time: " << elapsed_s << " s    \t, remaining time: " << (frames - i)*elapsed_s << "s\n";
		frame_times += elapsed_s;
		ppm_.save(filename_ + "_" + std::to_string(i) + ".ppm");
	}
		std::cout << "Rendertime total: " << frame_times << ", Rendertime per Frame: " << frame_times / frames;
}

Color Renderer::calculate_color(std::shared_ptr<Shape> shape, glm::vec3 const& cut, glm::vec3 const& normal, Scene const& scene) {
	Color ambient = calculate_ambiente(shape, scene);
	Color diffuse = calculate_diffuse(shape, cut, normal, scene);
	Color specular = calculate_specular(shape, cut, normal, scene);
	return  ambient + diffuse + specular;
}

//do you really have to multiply two colors?
Color Renderer::calculate_ambiente(std::shared_ptr<Shape> shape, Scene const& scene) {
	return scene.ambiente_->color_ * shape->material()->ka;
}

//seems to be working!!!
Color Renderer::calculate_diffuse(std::shared_ptr<Shape> shape, glm::vec3 const& cut, glm::vec3 const& normal, Scene const& scene) {
	Color comb_clr{ 0,0,0 };
	std::vector<Color> light_colors{};
	for (std::shared_ptr<Light> light : scene.light_vec_) {
		bool can_see_light = true;
		
		glm::vec3 cut_point, normal_new;
		glm::vec3 vec_to_light = glm::normalize(light->position_ - cut);
		float distance;

		for (std::shared_ptr<Shape> shape_ptr : scene.shape_vec_) {
			//the cut can be inside the shape so a point outside the shape is calculated by cut + 0.1f*normal
			bool cuts_shape = shape_ptr->intersect(Ray{ cut + 0.1f*normal,vec_to_light }, distance, cut_point, normal_new);
			if (cuts_shape) {
				can_see_light = false;
				break;
			}
		}
		if (can_see_light) {
			float o = glm::dot(vec_to_light, glm::normalize(normal));
			Color i_p = light->color_*light->brightness_;
			Color k_d = shape->material()->kd;
			light_colors.push_back(k_d * o *i_p);
		}
	}

	for (int i = 0; i < light_colors.size(); ++i) {
		Color clr = light_colors.at(i);
		comb_clr += clr;
	}
	return comb_clr;
}

//not completely working yet
Color Renderer::calculate_specular(std::shared_ptr<Shape> shape, glm::vec3 const& cut, glm::vec3 const& normal, Scene const& scene) {
	Color comb_clr{ 0,0,0 };
	std::vector<Color> light_colors{};
	for (std::shared_ptr<Light> light : scene.light_vec_) {
		bool can_see_light = true;

		glm::vec3 cut_point, normal_new;
		glm::vec3 vec_to_light = glm::normalize(light->position_ - cut);
		float distance;

		for (std::shared_ptr<Shape> shape_ptr : scene.shape_vec_) {
			//the cut can be inside the shape so a point outside the shape is calculated by cut + 0.1f*normal
			bool cuts_shape = shape_ptr->intersect(Ray{ cut + 0.5f*normal,vec_to_light }, distance, cut_point, normal_new);
			if (cuts_shape) {
				can_see_light = false;
				break;
			}
		}
		if (can_see_light) {
			float m = shape->material()->m;
			glm::vec3 v = glm::normalize(scene.camera_->position_ - cut);
			glm::vec3 r = glm::dot(normal,vec_to_light)*2.0f*normal-vec_to_light;
			float cos = pow(glm::dot(r,v), m);
			float m_pi = (m + 2) / (2 * M_PI);
			Color i_p = light->color_*light->brightness_;
			Color k_s = shape->material()->ks;
			light_colors.push_back(k_s * m_pi *cos *i_p);
		}
	}

	for (int i = 0; i < light_colors.size(); ++i) {
		Color clr = light_colors.at(i);
		comb_clr += clr;
	}
	return comb_clr;
}

//to check if the intersect methods work...
Color Renderer::calculate_depth_map(glm::vec3 const& cut, Scene const& scene, float const& max_dist) {
	Color color{ 1.0f, 1.0f, 1.0f };
	float value = glm::length(cut - scene.camera_->position_) / max_dist;
	return Color{ 1.0f, 1.0f, 1.0f } - color*value;
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
