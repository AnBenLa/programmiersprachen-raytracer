// -----------------------------------------------------------------------------
// Copyright  : (C) 2014-2017 Andreas-C. Bernstein
// License    : MIT (see the file LICENSE)
// Maintainer : Andreas-C. Bernstein <andreas.bernstein@uni-weimar.de>
// Stability  : experimental
//
// Renderer
// -----------------------------------------------------------------------------

#ifndef BUW_RENDERER_HPP
#define BUW_RENDERER_HPP

#include "color.hpp"
#include "pixel.hpp"
#include "ppmwriter.hpp"
#include "scene.hpp"
#include <string>
#include <glm/glm.hpp>

class Renderer
{
public:
  Renderer(unsigned w, unsigned h, std::string const& file);

  void render(Scene const& s, int frames);
  void write(Pixel const& p);
  Color calculate_color(std::shared_ptr<Shape> shape, glm::vec3 const& cut, glm::vec3 const& normal, Scene const& scene, Ray const& ray, int step);
  Color calculate_reflection(std::shared_ptr<Shape> shape, glm::vec3 const& cut, glm::vec3 const& normal, Scene const& scene, Ray const& ray, int step);
  Color calculate_refraction(std::shared_ptr<Shape> shape, glm::vec3 const& cut, glm::vec3 const& normal, Scene const& scene, Ray const& ray, int step);
  Color calculate_ambiente(std::shared_ptr<Shape> shape, Scene const& scene);
  Color calculate_diffuse(std::shared_ptr<Shape> shape, glm::vec3 const& cut, glm::vec3 const& normal, Scene const& scene);
  Color calculate_specular(std::shared_ptr<Shape> shape, glm::vec3 const& cut, glm::vec3 const& normal, Scene const& scene);
  Color calculate_depth_map(glm::vec3 const& cut, Scene const& scene, float const& max_dist);
  void tone_mapping(Color& color);

  inline std::vector<Color> const& color_buffer() const
  {
    return color_buffer_;
  }

private:
  unsigned width_;
  unsigned height_;
  std::vector<Color> color_buffer_;
  std::string filename_;
  PpmWriter ppm_;

  //calculates the mix_values of Refraction + Reflection 
  void fresnel(float refraction_index,glm::vec3 const& normal_Hit,glm::vec3 const& incoming_vector,float& refr_mix);
};

Ray transformRay(glm::mat4 const& mat, Ray const& ray);

#endif // #ifndef BUW_RENDERER_HPP
