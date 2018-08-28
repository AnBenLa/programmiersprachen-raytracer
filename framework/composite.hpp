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
        bool intersect(Ray const& incoming_ray,float& distance,glm::vec3& cut_point, glm::vec3& normal_vec, std::shared_ptr<Shape> shape)const;
        std::ostream& print(std::ostream& os)const;
        
        //getter
        std::vector<std::shared_ptr<Shape>>& getShapes(std::vector<std::shared_ptr<Shape>>& shapes);
        std::shared_ptr<BoundingBox> boundingBox()const;
        std::string name()const;

    private:

        std::vector<std::shared_ptr<Shape>>shapes_;
        std::vector<std::shared_ptr<Composite>>composites_;
    
        //calculates bounding box for the whole composite for performance
        void updateBoundingBox(); 
};

#endif