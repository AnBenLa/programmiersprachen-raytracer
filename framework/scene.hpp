#ifndef SCENE_STRUCT
#define SCENE_STRUCT
#include <fstream>
#include <iostream>
#include "material.hpp"
#include "box.hpp"
#include <vector>
#include <set>
#include <map>
#include <string>

struct Scene {
	std::map<std::string, std::shared_ptr<Material>> mat_map;
	std::map<std::string, std::shared_ptr<Shape>> shape_map;
};

void deserializeObjects(Scene& scene, std::string line){
	std::vector<std::string> lineParts;
	std::stringstream iss(line);
	std::string word;

	while(iss >> word){
		lineParts.push_back(word);
	}

	if(lineParts[1]=="material")
	{
		std::cout<<"Load material ...\n";
				//creates a new material
		try{
			Color ka{std::stof(lineParts[3],NULL),std::stof(lineParts[4],NULL),std::stof(lineParts[5],NULL)};
			Color kd{std::stof(lineParts[6],NULL),std::stof(lineParts[7],NULL),std::stof(lineParts[8],NULL)};
			Color ks{std::stof(lineParts[9],NULL),std::stof(lineParts[10],NULL),std::stof(lineParts[11],NULL)};
			float m{std::stof(lineParts[12],NULL)};
			std::shared_ptr<Material>mat = std::make_shared<Material>(lineParts[2],ka,kd,ks,m); 
			std::cout<<"Material: " << *mat<<"\n";
			scene.mat_map.insert(std::pair<std::string,std::shared_ptr<Material>>(mat->name,mat));
		}
		catch(std::invalid_argument arg)
		{	
			std::cout<<"Something went wrong, while loading material. Check format!\n";
			std::cout<<"Throws exception : "<<arg.what()<<"\n";
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
				std::shared_ptr<Material> mat = scene.mat_map.at(lineParts[10]);
				std::shared_ptr<Box> box = std::make_shared<Box>( min, max, name, mat);
				scene.shape_map.insert(std::pair<std::string, std::shared_ptr<Shape>>(name, box));
				std::cout << "Box: " << *box << "\n";
			}
			catch (std::invalid_argument arg)
			{
				std::cout << "Something went wrong, while loading the box. Check format!\n";
				std::cout << "Throws exception : " << arg.what() << "\n";
			}
		}
	}
}

void readSDF_File(std::string const& path,Scene& scene) {

	//opens ifstream, to read file
	std::ifstream ifs(path);
	if (!ifs.is_open()) {
		std::cout << "\nFailed to open file at : "<< path << "\n" << std::endl;
	}
	else {
		std::cout << "Opened OK" << std::endl;
		std::string line;
		while(std::getline(ifs,line)){
			deserializeObjects(scene, line);
		}
	}
};

bool operator <(std::shared_ptr<Material> const& lhs, std::shared_ptr<Material>const& rhs)
{
	return lhs->name<rhs->name;
}

std::shared_ptr<Material> search_for_material(std::string name, Scene& scene){
	
	auto it = scene.mat_map.find(name);
	if(it != scene.mat_map.end()){
		return scene.mat_map.find(name)->second;
	}	
	else
	{
		return nullptr;
	}
}
#endif