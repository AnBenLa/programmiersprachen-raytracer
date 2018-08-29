#include "composite.hpp"
#include <algorithm>

Composite::Composite() : Shape{"no name", nullptr} {};

Composite::Composite(std::string const& name) : Shape{ name, nullptr } {};

Composite::Composite(std::string const& name,std::vector<std::shared_ptr<Shape>>const& shapes,std::vector<std::shared_ptr<Composite>>const& composites):
    Shape{name, nullptr},
    shapes_{shapes},
    composites_{composites}
{
    updateBoundingBox();
};

Composite::~Composite(){};

//adds another shape to the composite
void Composite::add(std::shared_ptr<Shape> const& child)
{
    shapes_.push_back(child);
}

void Composite::add(std::shared_ptr<Composite> const& child)
{
    composites_.push_back(child);
}

bool Composite::intersect(Ray const& incoming_ray, float& distance,glm::vec3& cut_point, glm::vec3& normal_vec, std::shared_ptr<Shape>& shape)const
{
	float closest_distance = 0.0f;
	std::shared_ptr<Shape> closest_shape;
	glm::vec3 closest_normal, closest_cut;
	bool first_dist_set = false;
    
    //first check BoundingBox intersection, then children intersection
    if(boundingBox_->intersect(incoming_ray))
    {
        //check composites for intersection
        for(std::shared_ptr<Composite> child : composites_)
        {
			float current_dist = 0.0f;
			glm::vec3 current_cut, current_norm;
			std::shared_ptr<Shape> current_shape;

            if(child->intersect(incoming_ray, current_dist, current_cut, current_norm, current_shape))
            {
                if(current_dist < closest_distance || !first_dist_set)
                {
					closest_shape = current_shape;
					closest_distance = current_dist;
					closest_normal = current_norm;
					closest_cut = current_cut;
					first_dist_set = true;
                };
            }
        }
        
        //check shapes for intersection
        for(std::shared_ptr<Shape> child : shapes_)
        {
			float current_dist = 0.0f;
			glm::vec3 current_cut, current_norm;
			std::shared_ptr<Shape> current_shape;
            if(child->intersect(incoming_ray, current_dist, current_cut, current_norm, current_shape))
            {
                if(current_dist < closest_distance || !first_dist_set)
                {
					closest_shape = child;
					closest_distance = current_dist;
					closest_normal = current_norm;
					closest_cut = current_cut;
					first_dist_set = true;
                };
            }          
        }
    }
	if (first_dist_set) {
		distance = closest_distance;
		cut_point = closest_cut;
		normal_vec = closest_normal;
		shape = closest_shape;
		return true;
	}
	else {
		return false;
	}
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
    os<<"The composite"<<this->name()<<"contains: \n";
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

std::shared_ptr<BoundingBox> Composite::boundingBox()const
{
    return boundingBox_;
}

//extends the BoundingBox if neccessary
void Composite::updateBoundingBox()
{
    //vector, which contains all bounding boxes from shapes
    std::vector<std::shared_ptr<BoundingBox>> boundingBoxes;

    for(std::shared_ptr<Composite> child : composites_)
    {
        boundingBoxes.push_back(child->boundingBox());
    }

    for(std::shared_ptr<Shape> child : shapes_)
    {
        boundingBoxes.push_back(child->boundingBox());
    }

	glm::vec3 min_bbox;
	glm::vec3 max_bbox;

    //get box_min
	if (boundingBoxes.size() > 1) {
		
		std::sort(boundingBoxes.begin(), boundingBoxes.end(), [](std::shared_ptr<BoundingBox> box1, std::shared_ptr<BoundingBox> box2)
			->bool {return box1->min_.x < box2->min_.x; });
		min_bbox.x = (*boundingBoxes.begin())->min_.x;

		std::sort(boundingBoxes.begin(), boundingBoxes.end(), [](std::shared_ptr<BoundingBox> box1, std::shared_ptr<BoundingBox> box2)
			->bool {return box1->min_.y < box2->min_.y; });
		min_bbox.y = (*boundingBoxes.begin())->min_.y;

		std::sort(boundingBoxes.begin(), boundingBoxes.end(), [](std::shared_ptr<BoundingBox> box1, std::shared_ptr<BoundingBox> box2)
			->bool {return box1->min_.z < box2->min_.z; });
		min_bbox.z = (*boundingBoxes.begin())->min_.z;

		//get box max
		std::sort(boundingBoxes.begin(), boundingBoxes.end(), [](std::shared_ptr<BoundingBox> box1, std::shared_ptr<BoundingBox> box2)
			->bool {return box1->max_.x > box2->max_.x; });
		max_bbox.x = (*boundingBoxes.begin())->max_.x;

		std::sort(boundingBoxes.begin(), boundingBoxes.end(), [](std::shared_ptr<BoundingBox> box1, std::shared_ptr<BoundingBox> box2)
			->bool {return box1->max_.y > box2->max_.y; });
		max_bbox.y = (*boundingBoxes.begin())->max_.y;

		std::sort(boundingBoxes.begin(), boundingBoxes.end(), [](std::shared_ptr<BoundingBox> box1, std::shared_ptr<BoundingBox> box2)
			->bool {return box1->max_.z > box2->max_.z; });
		max_bbox.z = (*boundingBoxes.begin())->max_.z;
	}
	else {
		min_bbox = boundingBoxes.at(0)->min_;
		max_bbox = boundingBoxes.at(0)->max_;
	}
    boundingBox_ = std::make_shared<BoundingBox>(min_bbox,max_bbox);
}