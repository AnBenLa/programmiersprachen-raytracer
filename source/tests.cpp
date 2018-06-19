#define CATCH_CONFIG_RUNNER
#include <catch.hpp>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/intersect.hpp>
#include "../framework/sphere.hpp"
#include "../framework/box.hpp"
#include "../framework/color.hpp"
#include "../framework/shape.hpp"

int main(int argc, char *argv[])
{
	Catch::Session().run(argc, argv);
	std::cin.get();
	return 0;
}

TEST_CASE("Sphere test") {
	glm::vec3 position{ 0.0,1.0,3.0 };
	Color color{ 1.0,1.0,0.0 };
	std::string name = "Toto";
	auto a = std::make_shared<Sphere>(position, 5.0, name, color);

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


	SECTION("name and color test") {
		REQUIRE(a->color().r == 1.0);
		REQUIRE(a->color().g == 1.0);
		REQUIRE(a->color().b == 0.0);
		REQUIRE(a->name().compare("Toto") == 0.0);
	}

	SECTION("print out test") {
		std::cout << *a;
	}
}

TEST_CASE("Box test") {
	glm::vec3 min{ 0.0,1.0,3.0 };
	glm::vec3 max{ 1.0,2.0,10.0 };
	Color color{ 1.0,0.0,1.0 };
	std::string name = "Africa";
	auto a = std::make_shared<Box>(min, max, name, color);

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

	SECTION("name and color test") {
		REQUIRE(a->color().r == 1.0);
		REQUIRE(a->color().g == 0.0);
		REQUIRE(a->color().b == 1.0);
		REQUIRE(a->name().compare("Africa") == 0.0);
	}

	SECTION("print out test"){
		std::cout << *a;
	}
}

//Wie genau ist das gemeint?
TEST_CASE(" intersect_ray_sphere ")
{
	glm::vec3 position{ 0.0,1.0,3.0 };
	Color color{ 1.0,1.0,0.0 };
	std::string name = "Toto";
	auto a = std::make_shared<Sphere>(position, 5.0, name, color);
	
	REQUIRE(a->intersect(*(new Ray{})));


	glm::vec3 ray_origin{ 0.0f, 0.0f, 0.0f };
	glm::vec3 ray_direction{ 0.0f, 0.0f, 1.0f };
	glm::vec3 sphere_center{ 0.0f ,0.0f, 5.0f };
	float sphere_radius{ 1.0f };
	float distance = 0.0f;
	auto result = glm::intersectRaySphere(ray_origin, ray_direction, sphere_center, sphere_radius * sphere_radius, distance);
	
	REQUIRE(distance == Approx(4.0f));
}

TEST_CASE("constructor and destructor order") {
	std::cout << "-------------------------------------------------------\n";
	Color red{ 255 , 0 , 0 };
	glm::vec3 position{ 0.0f, 0.0f, 0.0f };
	Sphere * s1 = new Sphere{ position , 1.2f, " sphere0 ", red };
	Shape * s2 = new Sphere{ position , 1.2f, " sphere1 ", red };
	s1 -> print(std::cout);
	s2 -> print(std::cout);

	delete s1;
	delete s2;
}