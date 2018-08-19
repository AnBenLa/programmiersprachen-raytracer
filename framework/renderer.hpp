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
#include "hit.hpp"
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
  Hit get_closest_hit(Scene const& scene, Ray const& ray);
  Color calculate_color(std::shared_ptr<Shape> shape, glm::vec3 const& cut, glm::vec3 const& normal, Scene const& scene, Ray const& ray);
  Color calculate_reflection(std::shared_ptr<Shape> shape, glm::vec3 const& cut, glm::vec3 const& normal, Scene const& scene, Ray const& ray);
  Color calculate_ambiente(std::shared_ptr<Shape> shape, Scene const& scene);
  Color calculate_diffuse(std::shared_ptr<Shape> shape, glm::vec3 const& cut, glm::vec3 const& normal, Scene const& scene);
  Color calculate_specular(std::shared_ptr<Shape> shape, glm::vec3 const& cut, glm::vec3 const& normal, Scene const& scene);
  Color calculate_depth_map(glm::vec3 const& cut, Scene const& scene, float const& max_dist);

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
};

#endif // #ifndef BUW_RENDERER_HPP
