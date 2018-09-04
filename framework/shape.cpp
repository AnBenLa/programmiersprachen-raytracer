#include <iostream>
#include <string>

#include "shape.hpp"
#include "color.hpp"

#define TRANSLATE_MATRIX glm::mat4x4{glm::vec4{1.0f,0.0f,0.0f,0.0f},glm::vec4{0.0f,1.0f,0.0f,0.0f},glm::vec4{0.0f,0.0f,1.0f,0.0f},glm::vec4{translate.x,translate.y,translate.z,1.0f}}
#define SCALE_MATRIX glm::mat4x4{glm::vec4{scale.x,0.0f,0.0f,0.0f},glm::vec4{0.0f,scale.y,0.0f,0.0f},glm::vec4{0.0f,0.0f,scale.z,0.0f},glm::vec4{0.0f,0.0f,0.0f,1.0f}}
#define XROT_MATRIX  glm::mat4x4{glm::vec4{1,0.0f,0.0f,0.0f},glm::vec4{0.0f,glm::cos(rotation),glm::sin(rotation),0.0f},glm::vec4{0.0f,-glm::sin(rotation),glm::cos(rotation),0.0f},glm::vec4{0.0f,0.0f,0.0f,1.0f}}
#define YROT_MATRIX	 glm::mat4x4{glm::vec4{glm::cos(rotation),0.0f,-glm::sin(rotation),0.0f},glm::vec4{0.0f,1.0f,0.0f,0.0f},glm::vec4{glm::sin(rotation),0.0f,glm::cos(rotation),0.0f},glm::vec4{0.0f,0.0f,0.0f,1.0f}}
#define ZROT_MATRIX  glm::mat4x4{glm::vec4{glm::cos(rotation),glm::sin(rotation),0.0f,0.0f},glm::vec4{-glm::sin(rotation),glm::cos(rotation),0.0f,0.0f},glm::vec4{0.0f,0.0f,1.0f,0.0f},glm::vec4{0.0f,0.0f,0.0f,1.0f}}

Shape::Shape(std::string const& name, std::shared_ptr<Material> const& material) : 
name_{ name },
material_{ material},
boundingBox_{nullptr}{
    world_transformation_= glm::mat4x4{glm::vec4{1.0f,0.0f,0.0f,0.0f},glm::vec4{0.0f,1.0f,0.0f,0.0f},glm::vec4{0.0f,0.0f,1.0f,0.0f},glm::vec4{0.0f,0.0f,0.0f,1.0f}};
    world_transformation_inv_ = glm::inverse(world_transformation_);
};

Shape::~Shape() {};

std::ostream& Shape::print(std::ostream& os) const 
{ return os << "Shape name: " << name_ << " Shape material: " << *material_; };

void Shape::apply_transformation(glm::vec3 const& translate,float rotation, Axis axis,glm::vec3 const& scale)
{
	glm::mat4x4 transla = TRANSLATE_MATRIX * XROT_MATRIX;
    //create TRS matrix
    //determines rotationaxis
    switch(axis)
    {
        case x_axis:
		world_transformation_= world_transformation_ * TRANSLATE_MATRIX*XROT_MATRIX*SCALE_MATRIX;
		break;

        case y_axis:
        world_transformation_= world_transformation_ * TRANSLATE_MATRIX*YROT_MATRIX*SCALE_MATRIX;
		break;

        case z_axis:
        world_transformation_= world_transformation_ * TRANSLATE_MATRIX*ZROT_MATRIX*SCALE_MATRIX;
    }
    world_transformation_inv_ = glm::inverse(world_transformation_);
}

std::string Shape::name() const { return name_; };

std::shared_ptr<Material> Shape::material() const { return material_; };

std::ostream& operator <<(std::ostream& os, Shape const& s)
{	return s.print(os); }