#define CATCH_CONFIG_RUNNER
#include <catch.hpp>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/intersect.hpp>
#include "../framework/sphere.hpp"
#include "../framework/box.hpp"
#include "../framework/color.hpp"
#include "../framework/shape.hpp"
#include "../framework/scene.hpp" 

int main(int argc, char *argv[])
{
	Catch::Session().run(argc, argv);
	std::cin.get();
	return 0;
}

TEST_CASE("Sphere test") {
	glm::vec3 position{ 0.0,1.0,3.0 };
	Color color{ 1.0,1.0,0.0 };
	std::shared_ptr<Material> material = std::make_shared<Material>("Test",color,color,color,10);
	std::string name = "Toto";
	auto a = std::make_shared<Sphere>(position, 5.0, name, material);

	SECTION("Constructor test") {
		REQUIRE(a->radius() == 5.0);
		REQUIRE(a->center().x == 0.0);
		REQUIRE(a->center().y == 1.0);
		REQUIRE(a->center().z == 3.0);
	}

	SECTION("area and volume test") {
		REQUIRE(a->area() == Approx(314.1592));
		REQUIRE(a->volume() == Approx(523.599));
	}

	SECTION("print out test") {
		std::cout << *a;
	}
}

TEST_CASE("Box test") {
	glm::vec3 min{ 0.0,1.0,3.0 };
	glm::vec3 max{ 1.0,2.0,10.0 };
	Color color{ 1.0,0.0,1.0 };
	std::shared_ptr<Material> material = std::make_shared<Material>("Test", color, color, color, 10);
	std::string name = "Africa";
	auto a = std::make_shared<Box>(min, max, name, material);

	SECTION("Constructor test") {
		REQUIRE(a->min().x == 0.0);
		REQUIRE(a->min().y == 1.0);
		REQUIRE(a->min().z == 3.0);

		REQUIRE(a->max().x == 1.0);
		REQUIRE(a->max().y == 2.0);
		REQUIRE(a->max().z == 10.0);
	}

	SECTION("area and volume test") {
		REQUIRE(a->area() == Approx(30.0));
		REQUIRE(a->volume() == Approx(7));
	}

	SECTION("print out test"){
		std::cout << *a;
	}

	SECTION("Intersection test") {
		//ray doesnt cut box
		float distance = 0.0f;
		REQUIRE(!a->intersect(*new Ray{}, distance));

		//ray cuts box which is in front
		glm::vec3 min_1{ -1.0,-1.0,-2.0 };
		glm::vec3 max_1{ 1.0,1.0,-3.0 };
		auto b = std::make_shared<Box>(min_1, max_1, name, material);
		REQUIRE(b->intersect(*new Ray{}, distance));
		REQUIRE(distance > 0);

		//ray cuts box but its behind
		glm::vec3 min_2{ -1.0,-1.0, 2.0 };
		glm::vec3 max_2{ 1.0,1.0, 3.0 };
		auto c = std::make_shared<Box>(min_2, max_2, name, material);
		REQUIRE(c->intersect(*new Ray{}, distance));
		REQUIRE(distance < 0);

		//ray inside box 
		glm::vec3 min_3{ -1.0,-1.0, 1.0 };
		glm::vec3 max_3{ 1.0,1.0, -4.0 };
		auto d = std::make_shared<Box>(min_3, max_3, name, material);
		REQUIRE(d->intersect(*new Ray{}, distance));
		REQUIRE(distance > 0);
	}
}

TEST_CASE("Scene test") {
	std::shared_ptr<Scene> scene = std::make_shared<Scene>();
	readSDF_File("C:\\Users\\Mortiferum\\VisualStudioProjects\\Übung - 5\\programmiersprachen - raytracer\\Übung - 6\\Example.sdf");
}

TEST_CASE(" intersect_ray_sphere ")
{
	glm::vec3 position{ 0.0f, 0.0f, 5.0f };
	Color color{ 1.0,1.0,0.0 };
	std::string name = "Toto";
	std::shared_ptr<Material> material = std::make_shared<Material>("Test", color, color, color, 10);
	auto a = std::make_shared<Sphere>(position, 1.0f, name, material);
	float distance = 0.0f;
	a->intersect(*(new Ray{}), distance);
	REQUIRE(distance == Approx(-4.0f));

	glm::vec3 ray_origin{ 0.0f, 0.0f, 0.0f };
	glm::vec3 ray_direction{ 0.0f, 0.0f, 1.0f };
	glm::vec3 sphere_center{ 0.0f ,0.0f, 5.0f };
	float sphere_radius{ 1.0f };
	distance = 0.0f;
	auto result = glm::intersectRaySphere(ray_origin, ray_direction, sphere_center, sphere_radius * sphere_radius, distance);
	
	REQUIRE(distance == Approx(4.0f));
}

TEST_CASE("constructor and destructor order") {
	std::cout << "-------------------------------------------------------\n";
	Color red{ 255 , 0 , 0 };
	std::shared_ptr<Material> material = std::make_shared<Material>("Test", red, red, red, 10);
	glm::vec3 position{ 0.0f, 0.0f, 0.0f };
	Sphere * s1 = new Sphere{ position , 1.2f, " sphere0 ", material };
	Shape * s2 = new Sphere{ position , 1.2f, " sphere1 ", material };
	s1 -> print(std::cout);
	s2 -> print(std::cout);

	delete s1;
	delete s2;
}
