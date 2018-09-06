#include "composite.hpp"
#include <algorithm>
#include "renderer.hpp"

#define TRANSLATE_MATRIX glm::mat4x4{glm::vec4{1.0f,0.0f,0.0f,0.0f},glm::vec4{0.0f,1.0f,0.0f,0.0f},glm::vec4{0.0f,0.0f,1.0f,0.0f},glm::vec4{translate.x,translate.y,translate.z,1.0f}}
#define SCALE_MATRIX glm::mat4x4{glm::vec4{scale.x,0.0f,0.0f,0.0f},glm::vec4{0.0f,scale.y,0.0f,0.0f},glm::vec4{0.0f,0.0f,scale.z,0.0f},glm::vec4{0.0f,0.0f,0.0f,1.0f}}
#define XROT_MATRIX  glm::mat4x4{glm::vec4{1,0.0f,0.0f,0.0f},glm::vec4{0.0f,glm::cos(rotation),glm::sin(rotation),0.0f},glm::vec4{0.0f,-glm::sin(rotation),glm::cos(rotation),0.0f},glm::vec4{0.0f,0.0f,0.0f,1.0f}}
#define YROT_MATRIX	 glm::mat4x4{glm::vec4{glm::cos(rotation),0.0f,-glm::sin(rotation),0.0f},glm::vec4{0.0f,1.0f,0.0f,0.0f},glm::vec4{glm::sin(rotation),0.0f,glm::cos(rotation),0.0f},glm::vec4{0.0f,0.0f,0.0f,1.0f}}
#define ZROT_MATRIX  glm::mat4x4{glm::vec4{glm::cos(rotation),glm::sin(rotation),0.0f,0.0f},glm::vec4{-glm::sin(rotation),glm::cos(rotation),0.0f,0.0f},glm::vec4{0.0f,0.0f,1.0f,0.0f},glm::vec4{0.0f,0.0f,0.0f,1.0f}}

Composite::Composite() : Shape{"no name", nullptr} {
    world_transformation_= glm::mat4x4{glm::vec4{1.0f,0.0f,0.0f,0.0f},glm::vec4{0.0f,1.0f,0.0f,0.0f},glm::vec4{0.0f,0.0f,1.0f,0.0f},glm::vec4{0.0f,0.0f,0.0f,1.0f}};
    world_transformation_inv_ = glm::inverse(world_transformation_);};

Composite::Composite(std::string const& name) : Shape{ name, nullptr } {
    world_transformation_= glm::mat4x4{glm::vec4{1.0f,0.0f,0.0f,0.0f},glm::vec4{0.0f,1.0f,0.0f,0.0f},glm::vec4{0.0f,0.0f,1.0f,0.0f},glm::vec4{0.0f,0.0f,0.0f,1.0f}};
    world_transformation_inv_ = glm::inverse(world_transformation_);};

Composite::Composite(std::string const& name,std::vector<std::shared_ptr<Shape>>const& shapes,std::vector<std::shared_ptr<Composite>>const& composites):
    Shape{name, nullptr},
    shapes_{shapes},
    composites_{composites}
{
    createBoundingBox();
    world_transformation_= glm::mat4x4{glm::vec4{1.0f,0.0f,0.0f,0.0f},glm::vec4{0.0f,1.0f,0.0f,0.0f},glm::vec4{0.0f,0.0f,1.0f,0.0f},glm::vec4{0.0f,0.0f,0.0f,1.0f}};
    world_transformation_inv_ = glm::inverse(world_transformation_);
};

Composite::~Composite(){};

//adds another shape to the composite
void Composite::add(std::shared_ptr<Shape> const& child)
{
    shapes_.push_back(child);
    updateBoundingBox(child);
}

void Composite::add(std::shared_ptr<Composite> const& child)
{
    composites_.push_back(child);
    updateBoundingBox(child);
}

bool Composite::intersect(Ray const& incoming_ray, float& distance,glm::vec3& cut_point, glm::vec3& normal_vec, std::shared_ptr<Shape>& shape)const
{
    Ray transformedRay = transformRay(world_transformation_inv_,incoming_ray);
	float closest_distance = 0.0f;
	std::shared_ptr<Shape> closest_shape;
	glm::vec3 closest_normal, closest_cut;
	bool first_dist_set = false;
    
    //first check BoundingBox intersection, then children intersection
    if(boundingBox_->intersect(transformedRay))
    {
        //check composites for intersection
        for(std::shared_ptr<Composite> child : composites_)
        {
			float current_dist = 0.0f;
			glm::vec3 current_cut, current_norm;
			std::shared_ptr<Shape> current_shape;

            if(child->intersect(transformedRay, current_dist, current_cut, current_norm, current_shape))
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
            if(child->intersect(transformedRay, current_dist, current_cut, current_norm, current_shape))
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

void Composite::apply_transformation(glm::vec3 const& translate,float rotation, Axis axis,glm::vec3 const& scale)
{
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

//creates a bounding box for the whole composite
void Composite::createBoundingBox()
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

//extends the BoundingBox if neccessary
void Composite::updateBoundingBox(std::shared_ptr<Shape>shape)
{
    if(shape->boundingBox()->min_.x < boundingBox_->min_.x)
    {
        if(shape->boundingBox()->min_.y < boundingBox_->min_.y)
        {
            if(shape->boundingBox()->min_.z < boundingBox_->min_.z)
            {
                boundingBox_->min_ = shape->boundingBox()->min_;
            }
        }
    }

    if(shape->boundingBox()->max_.x > boundingBox_->max_.x)
    {
        if(shape->boundingBox()->max_.y > boundingBox_->max_.y)
        {
            if(shape->boundingBox()->max_.z > boundingBox_->max_.z)
            {
                boundingBox_->max_ = shape->boundingBox()->max_;
            }
        }
    }
}

void Composite::updateBoundingBox(std::shared_ptr<Composite>composite)
{
    if(composite->boundingBox()->min_.x < boundingBox_->min_.x)
    {
        if(composite->boundingBox()->min_.y < boundingBox_->min_.y)
        {
            if(composite->boundingBox()->min_.z < boundingBox_->min_.z)
            {
                boundingBox_->min_ = composite->boundingBox()->min_;
            }
        }
    }

    if(composite->boundingBox()->max_.x > boundingBox_->max_.x)
    {
        if(composite->boundingBox()->max_.y > boundingBox_->max_.y)
        {
            if(composite->boundingBox()->max_.z > boundingBox_->max_.z)
            {
                boundingBox_->max_ = composite->boundingBox()->max_;
            }
        }
    }
}