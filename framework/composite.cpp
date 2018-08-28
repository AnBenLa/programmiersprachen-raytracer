#include "composite.hpp"
#include <algorithm>
#include <hit.hpp>

Composite::Composite(std::string const& name):
    name_{name}
{};

Composite::Composite(std::string const& name,std::vector<std::shared_ptr<Shape>>const& shapes,std::vector<std::shared_ptr<Composite>>const& composites):
    name_{name},
    shapes_{shapes},
    composites_{composites},
    boundingBox_{nullptr}
{
    updateBoundingBox();
};

//adds another shape to the composite
void Composite::add(std::shared_ptr<Shape> const& child)
{
    shapes_.push_back(child);
    updateBoundingBox();
}

void Composite::add(std::shared_ptr<Composite> const& child)
{
    composites_.push_back(child);
    updateBoundingBox();
}

std::string Composite::name()const
{
    return name_;
}

std::shared_ptr<Hit> Composite::intersect(Ray const& incoming_ray,glm::vec3& cut_point, glm::vec3& normal_vec)const
{
    std::shared_ptr<Hit>closestIntersection{nullptr};
    
    //first check BoundingBox intersection, then children intersection
    if(boundingBox_->intersect(incoming_ray,cut_point,normal_vec)!=nullptr)
    {
        //check composites for intersection
        for(std::shared_ptr<Composite> child : composites_)
        {
            std::shared_ptr<Hit>currentHit = child->intersect(incoming_ray,cut_point,normal_vec);
            if(currentHit!=nullptr)
            {
                if(currentHit<closestIntersection)
                {
                    closestIntersection = currentHit;
                };
            }
        }
        
        //check shapes for intersection
        for(std::shared_ptr<Shape> child : shapes_)
        {
            std::shared_ptr<Hit>currentHit = child->intersect(incoming_ray,cut_point,normal_vec);
            if(currentHit!=nullptr)
            {
                if(currentHit<closestIntersection)
                {
                    closestIntersection = currentHit;
                };
            }          
        }
    }
    return closestIntersection;    
}

std::vector<std::shared_ptr<Shape>>& Composite::getShapes(std::vector<std::shared_ptr<Shape>>& shapes)
{
    for(auto child : composites_)
    {
        for(auto child : child->getShapes(shapes))
        {
            shapes.push_back(child);
        }
    }

    for(auto child : shapes_){
        shapes.push_back(child);
    };

    return shapes;
}

std::ostream& Composite::print(std::ostream& os)const
{
    os<<"The composite"<<name<<"contains: \n";
    for(std::shared_ptr<Shape> child : shapes_)
    {
        child->print(os);
        os<<"\n";
    }

    for(std::shared_ptr<Composite> child : composites_)
    {
        child->print(os);
        os<<"\n";        
    }

    return os;
}

std::shared_ptr<Box> Composite::getBoundingBox()const
{
    return boundingBox_;
}

//extends the BoundingBox if neccessary
void Composite::updateBoundingBox()
{
    //vector, which contains all bounding boxes from shapes
    std::vector<std::shared_ptr<Box>> boundingBoxes;

    for(std::shared_ptr<Composite> child : composites_)
    {
        boundingBoxes.push_back(child->getBoundingBox());
    }

    for(std::shared_ptr<Shape> child : shapes_)
    {
        boundingBoxes.push_back(child->getBoundingBox());
    }

    //get box_min
    glm::vec3 min_bbox;
    std::sort(boundingBoxes.begin(),boundingBoxes.end(),[](std::shared_ptr<Box> box1,std::shared_ptr<Box> box2)
    ->bool{return box1->min().x<box2->min().x;});
    min_bbox.x = (*boundingBoxes.begin())->min().x;

    std::sort(boundingBoxes.begin(),boundingBoxes.end(),[](std::shared_ptr<Box> box1,std::shared_ptr<Box> box2)
    ->bool{return box1->min().y<box2->min().y;});
    min_bbox.y = (*boundingBoxes.begin())->min().y;

    std::sort(boundingBoxes.begin(),boundingBoxes.end(),[](std::shared_ptr<Box> box1,std::shared_ptr<Box> box2)
    ->bool{return box1->min().z<box2->min().z;});
    min_bbox.z = (*boundingBoxes.begin())->min().z;

    //get box max
    glm::vec3 max_bbox;
    std::sort(boundingBoxes.begin(),boundingBoxes.end(),[](std::shared_ptr<Box> box1,std::shared_ptr<Box> box2)
    ->bool{return box1->max().x<box2->max().x;});
    max_bbox.x = (*boundingBoxes.begin())->max().x;

    std::sort(boundingBoxes.begin(),boundingBoxes.end(),[](std::shared_ptr<Box> box1,std::shared_ptr<Box> box2)
    ->bool{return box1->max().y<box2->max().y;});
    max_bbox.y = (*boundingBoxes.begin())->max().y;

    std::sort(boundingBoxes.begin(),boundingBoxes.end(),[](std::shared_ptr<Box> box1,std::shared_ptr<Box> box2)
    ->bool{return box1->max().z<box2->max().z;});
    max_bbox.z = (*boundingBoxes.begin())->max().z;

    boundingBox_ = std::make_shared<Box>(min_bbox,max_bbox,name()+" BoundingBox",nullptr);
}