#ifndef SCENE_STRUCT
#define SCENE_STRUCT
#include <fstream>
#include <iostream>
#include "material.hpp"
#include <vector>
#include <set>
#include <map>

struct Scene {

	std::map<std::string, std::shared_ptr<Material>> mat_map;
	std::set<std::string, std::shared_ptr<Material>> mat_set;
	std::vector<std::shared_ptr<Material>> mat_vec;
};

void readSDF_File(std::string path) {
	std::ifstream ifs(path);
	if (!ifs.is_open()) {
		std::cout << "\nFailed to open file at : "<< path << "\n" << std::endl;
	}
	else {
		std::cout << "Opened OK" << std::endl;
	}
	for (std::string line; getline(ifs, line); )
	{
		std::cout << line;
	}
};
#endif