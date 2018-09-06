#ifndef COMPOSITE_HEADER
#define COMPOSITE_HEADER
#include <glm/vec3.hpp>
#include <string>
#include <vector>
#include "shape.hpp"
#include "box.hpp"
#include "ray.hpp"
#include "boundingbox.hpp"

class Composite : Shape
{
    public:
		Composite();
        Composite(std::string const& name);
        Composite(std::string const& name,std::vector<std::shared_ptr<Shape>> const& shapes,std::vector<std::shared_ptr<Composite>> const& composites);
        ~Composite();
        
        void add(std::shared_ptr<Shape>const& child);
        void add(std::shared_ptr<Composite>const& child);
        bool intersect(Ray const& incoming_ray,float& distance,glm::vec3& cut_point, glm::vec3& normal_vec, std::shared_ptr<Shape>& shape)const;
        std::ostream& print(std::ostream& os)const;
        
        //calculates bounding box for the whole composite for performance
        void createBoundingBox();

        //getter
        std::vector<std::shared_ptr<Shape>>& getShapes(std::vector<std::shared_ptr<Shape>>& shapes);
        std::shared_ptr<BoundingBox> boundingBox()const;

    private:

        std::vector<std::shared_ptr<Shape>>shapes_;
        std::vector<std::shared_ptr<Composite>>composites_;
        glm::mat4x4 world_transformation_;
	    glm::mat4x4 world_transformation_inv_;
        
        //calculates bounding box for the whole composite for performance
        void updateBoundingBox(std::shared_ptr<Shape>shape);
        void updateBoundingBox(std::shared_ptr<Composite>composite);
};

#endif