#ifndef SCENE_STRUCT
#define SCENE_STRUCT
#include <fstream>
#include <iostream>
#include "material.hpp"
#include <vector>
#include <set>
#include <map>
#include <string>

struct Scene {

	/*std::map<std::string, std::shared_ptr<Material>> mat_map;
	std::set<std::string, std::shared_ptr<Material>> mat_set;
	std::vector<std::shared_ptr<Material>> mat_vec;*/
};

void readSDF_File(std::string const& path) {

	//opens ifstream, to read file
	std::ifstream ifs(path);
	if (!ifs.is_open()) {
		std::cout << "\nFailed to open file at : "<< path << "\n" << std::endl;
	}
	else {
		std::cout << "Opened OK" << std::endl;
	}

   	std::vector<std::string>lineParts;
	//reads the file by line
	//splits the string in single arguments
	for (std::string line; getline(ifs, line,' '); )
	{
		if(int pos = line.find("\n")!=std::string::npos){
			lineParts.push_back(line.substr(0,pos+1));
			lineParts.push_back(line.substr(pos+2));
		}
		else{
			lineParts.push_back(line.c_str());
		}
	}

	//assigns file-arguments
	while(!lineParts.empty())
	{
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
				std::cout<<*mat<<"\n";
				//mat_vec.push_back(*mat);
				//mat_map.push_back()
			}
			catch(std::invalid_argument arg)
			{	
				std::cout<<"Something went wrong, while loading material. Check format!\n";
				std::cout<<"Throws exception : "<<arg.what()<<"\n";
			}
			catch(std::exception e)
			{
				std::cout<<"Something went wrong, while loading material. Check format!\n";
				std::cout<<"Throws exception : "<<e.what()<<"\n";
			}
			lineParts.erase(lineParts.begin(),lineParts.begin()+13);
		}	
		else
		{
			std::cout<<"Nothing to load!\n";
		}
	}
};
#endif