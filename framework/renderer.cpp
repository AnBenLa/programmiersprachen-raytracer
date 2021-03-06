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
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <ppl.h>
#endif


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
		//scene.camera_->position_ = glm::vec3{ 0, 0 , 200 - i };
		//scene.light_vec_.at(0)->position_ = glm::vec3{ 500,800,i*6 };

		//creates a rotating motion for the camera around the origin
	
		glm::vec3 distance{ 0.0f , 0.0f, 450.0f };
		glm::mat4x4 rotation = glm::rotate((float)i, glm::vec3{ 0.0f, 1.0f, 0.0f });
		glm::vec4 n_4 = glm::vec4{ scene.camera_->direction_, 0 } *rotation;
		glm::vec4 up_4 = glm::vec4{ scene.camera_->up_, 0 } *rotation;
		glm::vec3 n = glm::normalize(glm::vec3{ n_4.x, n_4.y , n_4.z });
		glm::vec3 up = glm::vec3{ up_4.x, up_4.y , up_4.z };
		glm::vec3 u = glm::normalize(glm::cross(n, up));
		glm::vec3 v = glm::normalize(glm::cross(u, n));
		float x = (glm::vec4{ distance, 1.0f } *rotation).x;
		float z = (glm::vec4{ distance, 1.0f } *rotation).z;
		scene.camera_->transformation_ = glm::mat4{
			glm::vec4{u,0.0f},
			glm::vec4{v,0.0f},
			glm::vec4{-n,0.0f},
			glm::vec4{x, 50.0f, z, 1.0f}
		};
	

		auto start = std::chrono::high_resolution_clock::now();
		int progress = 0;
		
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
		Concurrency::parallel_for(std::size_t(0), std::size_t(height_), [&](std::size_t y) {
#else
		for (int y = 0; y < height_; ++y) {
#endif	
			if (progress == height_ / 4) {
				std::cout << "25% - ";
			}
			else if (progress == height_ / 2) {
				std::cout << "50% - ";
			}
			else if (progress == (3 * height_) / 4) {
				std::wcout << "75%\n";
			}
			for (unsigned x = 0; x < width_; ++x) {
				Pixel p(x, y);
				//p.color = Color{ 0.2314f, 0.5137f, 0.7412f };

				//generate the camera ray
				glm::vec3 pos = scene.camera_->position_;
				glm::vec3 dir = glm::normalize(scene.camera_->direction_);
				
				std::vector<glm::vec3> dirs;
				bool basic_anti_aliasing = false;

				if (basic_anti_aliasing) {
					glm::vec3 dir_1 = dir + glm::vec3{ x - (0.5 * width_) + 0.5f,y - (0.5 * height_) + 0.5f,-d };
					glm::vec3 dir_2 = dir + glm::vec3{ x - (0.5 * width_) + 0.5f,y - (0.5 * height_) - 0.5f,-d };
					glm::vec3 dir_3 = dir + glm::vec3{ x - (0.5 * width_) - 0.5f,y - (0.5 * height_) + 0.5f,-d };
					glm::vec3 dir_4 = dir + glm::vec3{ x - (0.5 * width_) - 0.5f,y - (0.5 * height_) - 0.5f,-d };

					dirs = std::vector<glm::vec3>{ dir_1, dir_2, dir_3, dir_4 };
				} else {
					dir = dir + glm::vec3{ x - (0.5 * width_),y - (0.5 * height_),-d };
					dirs = std::vector<glm::vec3>{ dir };
				}

				for (int i = 0; i < dirs.size(); ++i) {

					dir = dirs.at(i);
					Ray ray{ pos , glm::normalize(dir) };
					ray = transformRay(scene.camera_->transformation_, ray);

					//calculate the first shape that gets hit
					glm::vec3 cut, normal;
					float distance;
					std::shared_ptr<Shape> cut_shape = nullptr;
					bool hit = scene.root_composite_->intersect(ray, distance, cut, normal, cut_shape);

					//if a shape is hit the pixel color is computed
					if (cut_shape != nullptr && hit) {
						//Color current_color = calculate_depth_map(cut, scene, 150);
						Color current_color = calculate_color(cut_shape, cut, normal, scene, ray, 3);
						//tone_mapping(current_color);
						p.color += current_color * (1.0f/dirs.size());
					} if (!hit) {
						p.color += Color{ 0.2314f, 0.5137f, 0.7412f }*(1.0f / dirs.size());
					}
				}
				write(p);
			}
			progress += 1;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
		});
#else
		}
#endif	
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

void Renderer::tone_mapping(Color& current_color) {
	float tone_r = current_color.r / (current_color.r + 1);
	float tone_g = current_color.g / (current_color.g + 1);
	float tone_b = current_color.b / (current_color.b + 1);
	current_color = Color{ tone_r,tone_g,tone_b };
}

Color Renderer::calculate_color(std::shared_ptr<Shape> shape, glm::vec3 const& cut, glm::vec3 const& normal, Scene const& scene, Ray const& ray, int step) {
	Color final_value{ 0.0f,0.0f,0.0f };
	Color ambient = calculate_ambiente(shape, scene);
	Color diffuse = calculate_diffuse(shape, cut, normal, scene);
	Color specular = calculate_specular(shape, cut, normal, scene);

	if (shape->material()->o > 0 && shape->material()->glossy > 0)
	{
		Color reflection = calculate_reflection(shape, cut, normal, scene, ray, step);
		Color phong = (ambient + diffuse) * (1 - shape->material()->glossy) + reflection * shape->material()->glossy + specular;
		Color refraction = calculate_refraction(nullptr,shape, cut, normal, scene, ray);

		final_value = phong * (1 - shape->material()->o) + refraction * shape->material()->o;
	}else if (shape->material()->glossy > 0) {
		Color reflection = calculate_reflection(shape, cut, normal, scene, ray, step);
	
		final_value = (ambient + diffuse) * (1 - shape->material()->glossy) + reflection * shape->material()->glossy + specular;
	}
	else if (shape->material()->o > 0) {
		Color refraction = calculate_refraction(nullptr, shape, cut, normal, scene, ray);
		Color phong = ambient + diffuse + specular;
		final_value = phong * (1 - shape->material()->o) + refraction * shape->material()->o;
	} else {
		final_value = ambient + diffuse + specular;
	}
	return  final_value;
}

Color Renderer::calculate_reflection(std::shared_ptr<Shape> shape, glm::vec3 const& cut, glm::vec3 const& normal, Scene const& scene, Ray const& ray, int step){
	glm::vec3 reflection_vec = glm::reflect(glm::normalize(ray.direction), glm::normalize(normal));
	Ray new_ray{ cut + 0.1f*normal, glm::normalize(reflection_vec) };
	glm::vec3 new_cut, new_normal;
	float distance;
	std::shared_ptr<Shape> cut_shape = nullptr;
	bool hit = scene.root_composite_->intersect(new_ray, distance ,new_cut, new_normal, cut_shape);

	if (!hit) {
		return Color{ 0.2314f, 0.5137f, 0.7412f };
	} else {
		if (step > 0) {
			Color reflected_color = calculate_color(cut_shape, new_cut, new_normal, scene, new_ray, step - 1);
			return reflected_color;
		}
		else {
			return Color{ 0,0,0 };
		}
	}
}

//calculates refraction 
Color Renderer::calculate_refraction(std::shared_ptr<Shape> prev_shape, std::shared_ptr<Shape> shape, glm::vec3 const& cut, glm::vec3 const& normal, Scene const& scene, Ray const& ray)
{
	float eta = 1.0f;
	float ior{shape->material()->n}; //refraction index

	if (prev_shape == nullptr) {
		eta = 1.0f / ior;
	} else if (prev_shape == shape) {
		eta = ior;
	} 

	//ior = 2.0f - ior;
	//calculates refraction-vector from outside-inside
	glm::vec3 refraction_vec = glm::normalize(glm::refract(glm::normalize(ray.direction),glm::normalize(normal),eta));
	glm::vec3 refraction_origin = cut-0.2f*normal;
	Ray new_ray{refraction_origin,refraction_vec};

	glm::vec3 new_cut, new_normal;
	float distance;
	std::shared_ptr<Shape> cut_shape = nullptr;

	//trace the ray and refract again inside - outside
	bool hit = scene.root_composite_->intersect(new_ray, distance ,new_cut, new_normal, cut_shape);
	if (hit && cut_shape != shape) {
		return calculate_color(cut_shape, new_cut, new_normal, scene, new_ray, 3);
	}
	else if (hit) {
		return calculate_refraction(shape, cut_shape, new_cut, -new_normal, scene, new_ray);
	} else {
		return Color{ 0.2314f, 0.5137f, 0.7412f };
	}
}

void Renderer::fresnel(float refraction_index,glm::vec3 const& normal_Hit,glm::vec3 const& incoming_ray ,float& refr_mix)
{
	float cosi = glm::clamp(-1.0f, 1.0f,glm::dot(incoming_ray,normal_Hit)); 
	float etai{1.0f};
	float etat{refraction_index};
	if(cosi>1)
	{
		std::swap(etai,etat);
	}
	float sint = etai/etat * sqrtf(std::max(0.f,1-cosi*cosi));
	if(sint>=1)
	{
		refr_mix = 1;
	}
	else
	{
		float cost = sqrtf(std::max(0.f, 1 - sint * sint));
		cosi = fabsf(cosi);
		float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
		float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
		refr_mix = (Rs * Rs + Rp * Rp) / 2; 
	}
}

Color Renderer::calculate_ambiente(std::shared_ptr<Shape> shape, Scene const& scene) {
	return scene.ambiente_->color_ * shape->material()->ka;
}

Color Renderer::calculate_diffuse(std::shared_ptr<Shape> shape, glm::vec3 const& cut, glm::vec3 const& normal, Scene const& scene) {
	Color comb_clr{ 0,0,0 };
	std::vector<Color> light_colors{};
	for (std::shared_ptr<Light> light : scene.light_vec_) {
		bool can_see_light = true;
		
		glm::vec3 cut_point, normal_new;
		glm::vec3 vec_to_light = glm::normalize(light->position_ - cut);
		float distance;
		std::shared_ptr<Shape> cut_shape = nullptr;

		//the cut can be inside the shape so a point outside the shape is calculated by cut + 0.1f*normal
		bool cuts_shape = scene.root_composite_->intersect(Ray{ cut + 0.2f*normal,vec_to_light }, distance ,cut_point, normal_new, cut_shape);
		if (cuts_shape && cut_shape != nullptr) {
			can_see_light = false;
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

Color Renderer::calculate_specular(std::shared_ptr<Shape> shape, glm::vec3 const& cut, glm::vec3 const& normal, Scene const& scene) {
	Color comb_clr{ 0,0,0 };
	std::vector<Color> light_colors{};
	for (std::shared_ptr<Light> light : scene.light_vec_) {
		bool can_see_light = true;

		glm::vec3 cut_point, normal_new;
		glm::vec3 vec_to_light = glm::normalize(light->position_ - cut);
		float distance;
		std::shared_ptr<Shape> cut_shape = nullptr;

		//the cut can be inside the shape so a point outside the shape is calculated by cut + 0.1f*normal
		bool cuts_shape = scene.root_composite_->intersect(Ray{ cut + 0.1f*normal,vec_to_light }, distance , cut_point, normal_new, cut_shape);
		if (cuts_shape && cut_shape != nullptr) {
			can_see_light = false;
		}
		if (can_see_light) {
			float m = shape->material()->m;
			glm::vec3 v = glm::normalize(scene.camera_->position_ - cut);
			glm::vec3 r = glm::dot(normal,vec_to_light)*2.0f*normal-vec_to_light;
			float p = glm::dot(r, v);
			if (p < 0)
				p = -p;
			float cos = pow(p, m);
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

Ray transformRay(glm::mat4 const& mat, Ray const& ray)
{
	//create homogen coords
	glm::vec4 origin{ray.origin,1};
	glm::vec4 direction{ray.direction,0};

	//transform ray
	origin = mat*origin;
	direction = mat*direction;
	
	//apply to Ray
	return Ray{glm::vec3{origin.x,origin.y,origin.z},glm::vec3{direction.x,direction.y,direction.z}};
}

