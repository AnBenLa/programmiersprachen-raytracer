#include <glm/vec3.hpp>
#include <glm/gtx/norm.hpp>
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
{};
Cylinder::~Cylinder(){};

double Cylinder::area()const
{
    double top_bottom = M_PI*pow(radius_,2);
    double side = 2*M_PI*glm::length(top_)*radius_;
    return 2*top_bottom+side;
}

double Cylinder::volume()const
{
    return M_PI*pow(radius_,2)*glm::length(top_);
}

std::ostream& Cylinder::print(std::ostream& os) const {
	return Shape::print(os)<<"Origin: ("<<base_.x<<","<<base_.y<<"), "<<"Height: ("<<top_.x<<","<<top_.y<<"), "<<"Radius: "<<radius_<<"\n";
}

//not ready yet
bool Cylinder::intersect(Ray const& ray, float& r, glm::vec3& cut_point, glm::vec3& normal)const
{
    //check intersection with infinite cylinder
    //used idea of https://www.gamedev.net/forums/topic/467789-raycylinder-intersection/
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
        //if x1=x2 quadratic equation has only one result (tangent)
        return true;
    }

    //check if the intersection is between planes (bottom/top)


    //check for intersection with planes (base)


    
    return true;
}