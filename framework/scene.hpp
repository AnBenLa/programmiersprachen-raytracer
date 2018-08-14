#ifndef SCENE_STRUCT
#define SCENE_STRUCT
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <sstream>

#include "material.hpp"
#include "shape.hpp"
#include "box.hpp"
#include "sphere.hpp"
#include "camera.hpp"
#include "light.hpp"
#include "ambiente.hpp"

struct Scene {
	std::map<std::string, std::shared_ptr<Material>> mat_map_;
	std::vector<std::shared_ptr<Shape>> shape_vec_;
	std::vector<std::shared_ptr<Light>> light_vec_;
	std::shared_ptr<Camera> camera_;
};

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
				std::shared_ptr<Material>mat = std::make_shared<Material>(lineParts[2], ka, kd, ks, m);
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