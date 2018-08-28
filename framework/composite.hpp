#ifndef COMPOSITE_HEADER
#define COMPOSITE_HEADER
#include <glm/vec3.hpp>
#include <string>
#include <vector>
#include "shape.hpp"
#include "box.hpp"
#include "ray.hpp"

class Composite
{
    public:

        Composite(std::string const& name);
        Composite(std::string const& name,std::vector<std::shared_ptr<Shape>> const& shapes,std::vector<std::shared_ptr<Composite>> const& composites);
        ~Composite();
        
        void add(std::shared_ptr<Shape>const& child);
        void add(std::shared_ptr<Composite>const& child);
        std::shared_ptr<Hit> intersect(Ray const& incoming_ray,glm::vec3& cut_point, glm::vec3& normal_vec)const;
        std::ostream& print(std::ostream& os)const;
        
        //getter
        std::vector<std::shared_ptr<Shape>>& getShapes(std::vector<std::shared_ptr<Shape>>& shapes);
        std::shared_ptr<Box> getBoundingBox()const;
        std::string name()const;

    private:

        std::vector<std::shared_ptr<Shape>>shapes_;
        std::vector<std::shared_ptr<Composite>>composites_;
        std::shared_ptr<Box>boundingBox_;
        std::string name_;
    
        //calculates bounding box for the whole composite for performance
        void updateBoundingBox(); 
};

#endif