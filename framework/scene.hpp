#ifndef SCENE_STRUCT
#define SCENE_STRUCT
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <iterator>

#include "material.hpp"
#include "shape.hpp"
#include "box.hpp"
#include "sphere.hpp"
#include "triangle.hpp"
#include "cone.hpp"
#include "camera.hpp"
#include "light.hpp"
#include "ambiente.hpp"

struct Scene {
	std::map<std::string, std::shared_ptr<Material>> mat_map_;
	std::vector<std::shared_ptr<Shape>> shape_vec_;
	std::vector<std::shared_ptr<Light>> light_vec_;
	std::shared_ptr<Ambiente> ambiente_;
	std::shared_ptr<Camera> camera_;
};

static void readMTL_File(std::string const& path, Scene& scene){
	std::ifstream ifs;
	ifs.open(path);
	if (!ifs.is_open()) {
		std::cout << "\nFailed to open file at : "<< path << "\n" << std::endl;
	}
	else {
		std::cout << "\nMTL Opened OK\n-----------------------------------------------------------------------------------------\n" << std::endl;
		std::string name;
		std::string line;
		Color ka{0.0f,0.0f,0.0f};
		Color kd{0.0f,0.0f,0.0f};
		Color ks{0.0f,0.0f,0.0f};
		float m = 0.0f;
		bool first_mtl = false;
		while(std::getline(ifs,line)){
			std::vector<std::string> lineParts;
			std::stringstream iss(line);
			std::string word;
			
			while(iss >> word){
				lineParts.push_back(word);
			}
			if(lineParts.size() > 1){
				if(lineParts[0] == "newmtl"){
					if(first_mtl){
						std::shared_ptr<Material> mat = std::make_shared<Material>(name, ka, kd, ks, m);
						std::cout << "Material: " << *mat << "\n";
						scene.mat_map_.insert(std::pair<std::string, std::shared_ptr<Material>>(mat->name, mat));
					}
					if(!first_mtl){
						first_mtl = true;
					}
					name = lineParts[1];
				}
				if(lineParts[0] == "Ka"){
					ka = Color{stof(lineParts[1]),stof(lineParts[2]),stof(lineParts[3])};
				}
				if(lineParts[0] == "Kd"){
					kd = Color{stof(lineParts[1]),stof(lineParts[2]),stof(lineParts[3])};
				}
				if(lineParts[0] == "Ks"){
					ks = Color{stof(lineParts[1]),stof(lineParts[2]),stof(lineParts[3])};
				}
				if(lineParts[0] == "Ns"){
					m = stof(lineParts[1]);
				}
			}		
		}
		if(first_mtl){
			std::shared_ptr<Material> mat = std::make_shared<Material>(name, ka, kd, ks, m);
			std::cout << "Material: " << *mat << "\n";
			scene.mat_map_.insert(std::pair<std::string, std::shared_ptr<Material>>(mat->name, mat));
		}
		std::cout << "\nMTL loaded\n-----------------------------------------------------------------------------------------\n" << std::endl;
	}
}

static void readOBJ_File(std::string const& path, Scene& scene){
	std::ifstream ifs;
	int shapes = 0;
	ifs.open(path);
	if (!ifs.is_open()) {
		std::cout << "\nFailed to open file at : "<< path << "\n" << std::endl;
	}
	else {
		std::cout << "\nOBJ Opened OK\n-----------------------------------------------------------------------------------------\n" << std::endl;
		std::string line;
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> normal_vertices;
		std::shared_ptr<Material> current_material;
		while(std::getline(ifs,line)){
			std::vector<std::string> lineParts;
			std::stringstream iss(line);
			std::string word;
			
			while(iss >> word){
				lineParts.push_back(word);
			}
			if(lineParts.size() > 1){
				if(lineParts[0] == "usemtl"){
					current_material = scene.mat_map_.at(lineParts[1]);
				}
				if(lineParts[0] == "v"){
					glm::vec3 vertex = glm::vec3{stof(lineParts[1]), -stof(lineParts[3]), stof(lineParts[2])};
					vertices.push_back(vertex);
				}
				if(lineParts[0] == "vn"){
					glm::vec3 vertex = glm::vec3{stof(lineParts[1]), stof(lineParts[2]), stof(lineParts[3])};
					normal_vertices.push_back(vertex);
				}
				if(lineParts[0] == "f"){
					if(lineParts.size() == 5){
						std::cout << shapes << "Error quad begin\n";
						std::string v_1 = lineParts[1];
						std::string v_2 = lineParts[2];
						std::string v_3 = lineParts[3];
						std::string v_4 = lineParts[4];

						std::string v_1_1 = v_1.substr(0, v_1.find("//"));
						v_1.erase(0, v_1.find("//") + 2);
						std::string v_2_1 = v_2.substr(0, v_2.find("//"));
						std::string v_3_1 = v_3.substr(0, v_3.find("//"));
						std::string v_4_1 = v_4.substr(0, v_4.find("//"));

						int v1 = stof(v_1_1) - 1;
						int normal = stof(v_1) - 1;
						int v2 = stof(v_2_1) - 1;
						int v3 = stof(v_3_1) - 1;
						int v4 = stof(v_4_1) - 1;

						std::cout << "\n" << v1 << ", " << v2  << ", "<< v3  << ", "<< v4  << ", "<< normal << "\n";
						std::cout << vertices.size() << ", " << normal_vertices.size() << "\n";

						std::shared_ptr<Triangle> tri_1 = std::make_shared<Triangle>(vertices.at(v1), vertices.at(v2), vertices.at(v3), normal_vertices.at(normal),"generic", current_material);
						std::shared_ptr<Triangle> tri_2 = std::make_shared<Triangle>(vertices.at(v1), vertices.at(v4), vertices.at(v3), normal_vertices.at(normal), "generic", current_material);
						scene.shape_vec_.push_back(tri_1);
						scene.shape_vec_.push_back(tri_2);
						shapes += 1;
						std::cout << shapes << "Error quad end\n";
					} else if(lineParts.size() == 4){
						std::cout << shapes << "Error tri begin\n";
						std::string v_1 = lineParts[1];
						std::string v_2 = lineParts[2];
						std::string v_3 = lineParts[3];

						std::string v_1_1 = v_1.substr(0, v_1.find("//"));
						v_1.erase(0, v_1.find("//") + 2);
						std::string v_2_1 = v_2.substr(0, v_2.find("//"));
						std::string v_3_1 = v_3.substr(0, v_3.find("//"));

						int v1 = stof(v_1_1) - 1;
						int normal = stof(v_1) - 1;
						int v2 = stof(v_2_1) - 1;
						int v3 = stof(v_3_1) - 1;


						std::shared_ptr<Triangle> tri = std::make_shared<Triangle>(vertices.at(v1), vertices.at(v2), vertices.at(v3), normal_vertices.at(normal),"generic", current_material);
						scene.shape_vec_.push_back(tri);
						shapes +=1;
						std::cout << shapes << "Error tri end\n";
					}
				}
			}
		}
		std::cout << vertices.size() << " vertices loaded\n";
		std::cout << shapes << " faces loaded\n";
		std::cout << "\nOBJ loaded\n-----------------------------------------------------------------------------------------\n" << std::endl;
	}
}

static void deserializeObjects(Scene& scene, std::string line){
	std::vector<std::string> lineParts;
	std::stringstream iss(line);
	std::string word;

	while(iss >> word){
		lineParts.push_back(word);
	}

	if (lineParts[0] == "define") {
		if (lineParts[1] == "material")
		{
			std::cout << "Load material ...\n";
			//creates a new material
			try {
				Color ka{ std::stof(lineParts[3],NULL),std::stof(lineParts[4],NULL),std::stof(lineParts[5],NULL) };
				Color kd{ std::stof(lineParts[6],NULL),std::stof(lineParts[7],NULL),std::stof(lineParts[8],NULL) };
				Color ks{ std::stof(lineParts[9],NULL),std::stof(lineParts[10],NULL),std::stof(lineParts[11],NULL) };
				float m{ std::stof(lineParts[12],NULL) };
				std::shared_ptr<Material>mat;
				if (lineParts.size() == 13) {
					mat = std::make_shared<Material>(lineParts[2], ka, kd, ks, m);
				} else {
					float glossy = std::stof(lineParts[13], NULL);
					mat = std::make_shared<Material>(lineParts[2], ka, kd, ks, m,glossy);
				}
				std::cout << "Material: " << *mat << "\n";
				scene.mat_map_.insert(std::pair<std::string, std::shared_ptr<Material>>(mat->name, mat));
			}
			catch (std::invalid_argument arg)
			{
				std::cout << "Something went wrong, while loading material. Check format!\n";
				std::cout << "Throws exception : " << arg.what() << "\n";
			}
		}
		if (lineParts[1] == "shape")
		{
			std::cout << "Load shape ...\n";
			if (lineParts[2] == "box") {
				try {
					std::string name = lineParts[3];
					glm::vec3 min = glm::vec3{ std::stof(lineParts[4], NULL), std::stof(lineParts[5], NULL), std::stof(lineParts[6], NULL) };
					glm::vec3 max = glm::vec3{ std::stof(lineParts[7], NULL), std::stof(lineParts[8], NULL), std::stof(lineParts[9], NULL) };
					std::shared_ptr<Material> mat = scene.mat_map_.at(lineParts[10]);
					std::shared_ptr<Box> box = std::make_shared<Box>(min, max, name, mat);
					scene.shape_vec_.push_back(box);
					std::cout << "Box: " << *box << "\n";
				}
				catch (std::invalid_argument arg)
				{
					std::cout << "Something went wrong, while loading the box. Check format!\n";
					std::cout << "Throws exception : " << arg.what() << "\n";
				}
			}
			if (lineParts[2] == "sphere") {
				try {
					std::string name = lineParts[3];
					glm::vec3 center = glm::vec3{ std::stof(lineParts[4], NULL), std::stof(lineParts[5], NULL), std::stof(lineParts[6], NULL) };
					int radius = std::stof(lineParts[7], NULL);
					std::shared_ptr<Material> mat = scene.mat_map_.at(lineParts[8]);
					std::shared_ptr<Sphere> sphere = std::make_shared<Sphere>(center, radius, name, mat);
					scene.shape_vec_.push_back(sphere);
					std::cout << "Sphere: " << *sphere << "\n";
				}
				catch (std::invalid_argument arg)
				{
					std::cout << "Something went wrong, while loading the sphere. Check format!\n";
					std::cout << "Throws exception : " << arg.what() << "\n";
				}
			}
			if (lineParts[2] == "triangle") {
				try {
					std::string name = lineParts[3];
					glm::vec3 a = glm::vec3{ std::stof(lineParts[4], NULL), std::stof(lineParts[5], NULL), std::stof(lineParts[6], NULL) };
					glm::vec3 b = glm::vec3{ std::stof(lineParts[7], NULL), std::stof(lineParts[8], NULL), std::stof(lineParts[9], NULL) };
					glm::vec3 c = glm::vec3{ std::stof(lineParts[10], NULL), std::stof(lineParts[11], NULL), std::stof(lineParts[12], NULL) };
					std::shared_ptr<Material> mat = scene.mat_map_.at(lineParts[13]);
					std::shared_ptr<Triangle> triangle = std::make_shared<Triangle>(a, b,  c,name, mat);
					scene.shape_vec_.push_back(triangle);
					std::cout << "Triangle: " << *triangle << "\n";
				}
				catch (std::invalid_argument arg)
				{
					std::cout << "Something went wrong, while loading the triangle. Check format!\n";
					std::cout << "Throws exception : " << arg.what() << "\n";
				}
			}
			if (lineParts[2] == "cone") {
				try {
					std::string name = lineParts[3];
					glm::vec3 base = glm::vec3{ std::stof(lineParts[4], NULL), std::stof(lineParts[5], NULL), std::stof(lineParts[6], NULL) };
					glm::vec3 peak = glm::vec3{ std::stof(lineParts[7], NULL), std::stof(lineParts[8], NULL), std::stof(lineParts[9], NULL) };
					float radius = std::stof(lineParts[10], NULL);
					std::shared_ptr<Material> mat = scene.mat_map_.at(lineParts[11]);
					std::shared_ptr<Cone> cone = std::make_shared<Cone>(base, peak, radius, name, mat);
					scene.shape_vec_.push_back(cone);
					std::cout << "Cone: " << *cone << "\n";
				}
				catch (std::invalid_argument arg)
				{
					std::cout << "Something went wrong, while loading the cone. Check format!\n";
					std::cout << "Throws exception : " << arg.what() << "\n";
				}
			}
		}
		if (lineParts[1] == "light") {
			std::cout << "Load light ...\n";
			try {
				std::string name = lineParts[2];
				glm::vec3 position = glm::vec3{ std::stof(lineParts[3], NULL), std::stof(lineParts[4], NULL), std::stof(lineParts[5], NULL) };
				Color color{ std::stof(lineParts[6], NULL), std::stof(lineParts[7], NULL), std::stof(lineParts[8], NULL) };
				double brightness = std::stof(lineParts[9], NULL);
				std::shared_ptr<Light> light = std::make_shared<Light>(name, position, color, brightness);
				scene.light_vec_.push_back(light);
				std::cout << "Light: " << *light << "\n";
			}
			catch (std::invalid_argument arg)
			{
				std::cout << "Something went wrong, while loading the light. Check format!\n";
				std::cout << "Throws exception : " << arg.what() << "\n";
			}
		}
		if (lineParts[1] == "camera") {
			std::cout << "Load camera ...\n";
			try {
				std::string name = lineParts[2];
				double fov = stof(lineParts[3]);
				glm::vec3 default_pos{ 0.0f, 0.0f, 0.0f };
				//always needs to has the norm 1
				glm::vec3 default_dir{ 0.0f, 0.0f, -1.0f };
				std::shared_ptr<Camera> camera = std::make_shared<Camera>(name, default_pos, default_dir, fov);
				scene.camera_ = camera;
				std::cout << "Camera: " << *camera << "\n";
			}
			catch (std::invalid_argument arg)
			{
				std::cout << "Something went wrong, while loading the camera. Check format!\n";
				std::cout << "Throws exception : " << arg.what() << "\n";
			}
		}
		if(lineParts[1] == "mtl"){
			std::string path = lineParts[2];
			readMTL_File(path, scene);
		}
		if(lineParts[1] == "obj"){
			std::string path = lineParts[2];
			readOBJ_File(path, scene);
		}
	}
	if (lineParts[0] == "ambient") {
		std::cout << "Load ambient ...\n";
		try {
			Color color{ std::stof(lineParts[1],NULL),std::stof(lineParts[2],NULL),std::stof(lineParts[3],NULL) };
			std::shared_ptr<Ambiente> am = std::make_shared<Ambiente>(color);
			scene.ambiente_ = am;
			std::cout << "Ambiente: " << *am << "\n";
		}
		catch (std::invalid_argument arg) {
			std::cout << "Something went wrong, while loading the ambiente. Check format!\n";
			std::cout << "Throws exception : " << arg.what() << "\n";
		}
	}
}

static void readSDF_File(std::string const& path,Scene& scene) {

	//opens ifstream, to read file
	std::ifstream ifs;
	ifs.open(path);
	if (!ifs.is_open()) {
		std::cout << "\nFailed to open file at : "<< path << "\n" << std::endl;
	}
	else {
		std::cout << "\nOpened OK\n-----------------------------------------------------------------------------------------\n" << std::endl;
		std::string line;
		while(std::getline(ifs,line)){
			deserializeObjects(scene, line);
		}
		std::cout << "\nScene loaded\n-----------------------------------------------------------------------------------------\n" << std::endl;
	}
};

static bool operator <(std::shared_ptr<Material> const& lhs, std::shared_ptr<Material>const& rhs)
{
	return lhs->name<rhs->name;
}

static std::shared_ptr<Material> search_for_material(std::string name, Scene& scene){
	
	auto it = scene.mat_map_.find(name);
	if(it != scene.mat_map_.end()){
		return scene.mat_map_.find(name)->second;
	}	
	else
	{
		return nullptr;
	}
}
#endif