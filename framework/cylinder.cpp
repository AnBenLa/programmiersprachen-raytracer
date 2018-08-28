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

Cylinder::Cylinder(glm::vec3 const& base, glm::vec3 const& top, double radius, std::string const& name, std::shared_ptr<Material> const& material):
    Shape{name, material},
    base_{base},
    top_{top},
    radius_{radius}
    {
        calculateBoundingBox();
    };

Cylinder::~Cylinder(){};

std::ostream& Cylinder::print(std::ostream& os) const {
	return Shape::print(os)<<"Base: ("<<base_.x<<","<<base_.y<<","<<base_.z<<"), "<<"Top: ("<<top_.x<<","<<top_.y<<","<<top_.z<<"), "<<"Radius: "<<radius_<<"\n";
}

//not ready yet
bool Cylinder::intersect(Ray const& ray, float& distance ,glm::vec3& cut_point, glm::vec3& normal, std::shared_ptr<Shape> shape)const
{
    //check intersection with infinite cylinder
    glm::vec3 AB = base_ - top_;
    glm::vec3 AO = ray.origin - base_;
    glm::vec3 AOxAB = glm::cross(AB,AO);
    glm::vec3 VxAB = glm::cross(ray.direction,AB);
    float ab2 = glm::dot(AB,AB);
    float a = glm::dot(VxAB,VxAB);
    float b = 2*glm::dot(VxAB,AOxAB);
    float c = glm::dot(AOxAB,AOxAB)-(radius_*radius_*ab2);

    //solve quadratic equation
    float p = pow(b,2)-4*a*c;
    if(p<0)
    {
        //quadratic equation has no result -> no intersection
        return false;
    }
    else{
        float x1 = (-b+sqrt(p))/2*a;
        float x2 = (-b-sqrt(p))/2*a;
    
        float x{0.0f};
        if(x1<x2){
            float x=x1;
        }
        else {float x = x2;}
        cut_point = ray.origin+ray.direction*x;

        //check if the intersection is between planes (bottom/top)
        float cut_distance1;
        float cut_distance2;
        glm::intersectRayPlane(cut_point,glm::normalize(AB),base_,AB,cut_distance1);
        glm::intersectRayPlane(cut_point,glm::normalize(AB),top_,AB,cut_distance2);
        cut_distance1 = glm::length(base_+AB*cut_distance1);
        cut_distance2 = glm::length(top_+AB*cut_distance2);

        if(cut_distance1+cut_distance2<=glm::length(AB))
        {
			distance = glm::length(cut_point - ray.origin);
			shape = std::make_shared<Cylinder>(base_, top_, radius_, name(), material());
			return true;
        }   
        else return false;    
    }
}

void Cylinder::calculateBoundingBox()
{
    glm::vec3 minBbox{base_.x-radius_,base_.y-radius_,base_.z-radius_};
    glm::vec3 maxBbox{top_.x+radius_,top_.y+radius_,top_.z+radius_};

    boundingBox_= std::make_shared<BoundingBox>(minBbox,maxBbox);
}

std::shared_ptr<BoundingBox> Cylinder::boundingBox() const {
	return boundingBox_;
}