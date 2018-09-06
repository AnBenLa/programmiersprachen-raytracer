#ifndef CYLINDER_HEADER
#define CYLINDER_HEADER
#include <glm/vec3.hpp>
#include "shape.hpp"
#include "color.hpp"
#include "ray.hpp"
#include "boundingbox.hpp"

class Cylinder : public Shape
{
    public:
        Cylinder(glm::vec3 base ,float height,float radius,std::string const& name,std::shared_ptr<Material> const& material);
        ~Cylinder();
        std::ostream& print(std::ostream& os)const override;
        bool intersect(Ray const& ray, float& ,glm::vec3&,glm::vec3&, std::shared_ptr<Shape>&)const override;

        //getter
        glm::vec3 base()const;
		float height() const;
        float radius() const;
        std::shared_ptr<BoundingBox> boundingBox() const override;

    private:
		glm::vec3 base_;
        float height_;
        float radius_;
        void calculateBoundingBox();
};
#endif