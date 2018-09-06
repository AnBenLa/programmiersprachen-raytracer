#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

#include "shape.hpp"
#include "color.hpp"

#define STANDARD_MATRIX glm::mat4x4{glm::vec4{1.0f,0.0f,0.0f,0.0f},glm::vec4{0.0f,1.0f,0.0f,0.0f},glm::vec4{0.0f,0.0f,1.0f,0.0f},glm::vec4{0.0f,0.0f,0.0f,1.0f}}
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

//returns actual boundingBox
std::shared_ptr<BoundingBox> Shape::boundingBox()const
{
    if(world_transformation_!=STANDARD_MATRIX)
    {
        //gets all 8 corners
        glm::vec3 A{boundingBox_->min_};
        glm::vec3 B{boundingBox_->min_.x,boundingBox_->max_.y,boundingBox_->min_.z};
        glm::vec3 C{boundingBox_->min_.x,boundingBox_->max_.y,boundingBox_->max_.z};
        glm::vec3 D{boundingBox_->min_.x,boundingBox_->min_.y,boundingBox_->max_.z};

        glm::vec3 E{boundingBox_->max_.x,boundingBox_->min_.y,boundingBox_->max_.z};
        glm::vec3 F{boundingBox_->max_.x,boundingBox_->min_.y,boundingBox_->min_.z};
        glm::vec3 G{boundingBox_->max_.x,boundingBox_->max_.y,boundingBox_->min_.z};
        glm::vec3 H{boundingBox_->max_};

        std::vector<glm::vec3>coords{A,B,C,D,E,F,G,H};

        //transforms the corners
        for(auto i = coords.begin();i<coords.end();i++)
        {
            glm::vec4 trans{*i,1};
            trans = world_transformation_*trans;
            i->x = trans.x;
            i->y = trans.y;
            i->z = trans.z;
        }

        //calculates minimum and maximum from transformed corners
        std::sort(coords.begin(),coords.end(),[](glm::vec3 const& v1,glm::vec3 const& v2)
        ->bool{
            return v1.x<v2.x;
        });
        boundingBox_->min_.x=coords.begin()->x;
        boundingBox_->max_.x=coords.at(coords.size()-1).x;

        std::sort(coords.begin(),coords.end(),[](glm::vec3 const& v1,glm::vec3 const& v2)
        ->bool{
            return v1.y<v2.y;
        });
        boundingBox_->min_.y=coords.begin()->y;
        boundingBox_->max_.y= coords.at(coords.size() - 1).y;

        std::sort(coords.begin(),coords.end(),[](glm::vec3 const& v1,glm::vec3 const& v2)
        ->bool{
            return v1.z<v2.z;
        });    
        boundingBox_->min_.z=coords.begin()->z;
        boundingBox_->max_.z= coords.at(coords.size() - 1).z;

        return boundingBox_;
    }
    else
    {
        return boundingBox_;
    }
}

std::ostream& operator <<(std::ostream& os, Shape const& s)
{	return s.print(os); }