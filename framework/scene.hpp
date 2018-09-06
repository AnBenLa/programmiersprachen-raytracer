#ifndef SCENE_STRUCT
#define SCENE_STRUCT
#define _USE_MATH_DEFINES
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <iterator>
#include <glm/gtx/transform.hpp>
#include <math.h>

#include "material.hpp"
#include "shape.hpp"
#include "box.hpp"
#include "sphere.hpp"
#include "triangle.hpp"
#include "cone.hpp"
#include "cylinder.hpp"
#include "camera.hpp"
#include "light.hpp"
#include "composite.hpp"
#include "ambiente.hpp"

struct Scene {
	std::map<std::string, std::shared_ptr<Material>> mat_map_;
	std::shared_ptr<Composite> root_composite_;
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

static void readOBJ_File(std::string const& path, Scene& scene, std::shared_ptr<Composite>& obj_comp){
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
		std::shared_ptr<Composite> current_comp = nullptr;
		std::string current_name;
		while(std::getline(ifs,line)){
			std::vector<std::string> lineParts;
			std::stringstream iss(line);
			std::string word;
			
			while(iss >> word){
				lineParts.push_back(word);
			}
			if(lineParts.size() > 1){
				if (lineParts[0] == "o") {
					std::string name = lineParts[1];
					if (current_comp != nullptr) {
						current_comp->updateBoundingBox();
						if (obj_comp == nullptr) {
							obj_comp = std::make_shared<Composite>();
						}
						obj_comp->add(current_comp);
						std::cout << "Comp: " << current_name << " was added\n";
					}
					current_comp = std::make_shared<Composite>(name);
					current_name = name;
				}
				if(lineParts[0] == "usemtl"){
					current_material = scene.mat_map_.at(lineParts[1]);
				}
				if(lineParts[0] == "v"){
					glm::vec3 vertex = glm::vec3{stof(lineParts[1]), -stof(lineParts[3]), stof(lineParts[2])};
					vertices.push_back(vertex);
				}
				if(lineParts[0] == "vn"){
					glm::vec3 vertex = glm::vec3{stof(lineParts[1]), -stof(lineParts[3]), stof(lineParts[2])};
					normal_vertices.push_back(vertex);
				}
				if(lineParts[0] == "f"){
					if(lineParts.size() == 5){
						std::string v_1 = lineParts[1];
						std::string v_2 = lineParts[2];
						std::string v_3 = lineParts[3];
						std::string v_4 = lineParts[4];

						std::string v_1_1 = v_1.substr(0, v_1.find("/"));
						v_1.erase(0, v_1.find("/") + 1);
						std::string v_2_1 = v_2.substr(0, v_2.find("/"));
						std::string v_3_1 = v_3.substr(0, v_3.find("/"));
						std::string v_4_1 = v_4.substr(0, v_4.find("/"));

						int v1 = stof(v_1_1) - 1;
						int normal = stof(v_1.substr(v_1.find("/") + 1)) - 1;
						int v2 = stof(v_2_1) - 1;
						int v3 = stof(v_3_1) - 1;
						int v4 = stof(v_4_1) - 1;

						std::shared_ptr<Triangle> tri_1 = std::make_shared<Triangle>(vertices.at(v1), vertices.at(v2), vertices.at(v3), normal_vertices.at(normal),"generic", current_material);
						std::shared_ptr<Triangle> tri_2 = std::make_shared<Triangle>(vertices.at(v1), vertices.at(v4), vertices.at(v3), normal_vertices.at(normal), "generic", current_material);
						current_comp->add(tri_1);
						current_comp->add(tri_2);
						shapes += 1;
					} else if(lineParts.size() == 4){
						std::string v_1 = lineParts[1];
						std::string v_2 = lineParts[2];
						std::string v_3 = lineParts[3];

						std::string v_1_1 = v_1.substr(0, v_1.find("//"));
						v_1.erase(0, v_1.find("/") + 1);
						std::string v_2_1 = v_2.substr(0, v_2.find("//"));
						std::string v_3_1 = v_3.substr(0, v_3.find("//"));

						int v1 = stof(v_1_1) - 1;
						int normal = stof(v_1.substr(v_1.find("/") + 1)) - 1;
						int v2 = stof(v_2_1) - 1;
						int v3 = stof(v_3_1) - 1;


						std::shared_ptr<Triangle> tri = std::make_shared<Triangle>(vertices.at(v1), vertices.at(v2), vertices.at(v3), normal_vertices.at(normal),"generic", current_material);
						current_comp->add(tri);
						shapes +=1;
					}
				}
			}
		}
		if (current_comp != nullptr) {
			current_comp->updateBoundingBox();
			obj_comp->add(current_comp);
			std::cout << "Comp: " << current_name << " was added\n";
		}
		obj_comp->updateBoundingBox();
		std::cout << "\n" << vertices.size() << " vertices loaded\n";
		std::cout << shapes << " faces loaded\n";
		std::cout << "\nOBJ loaded\n-----------------------------------------------------------------------------------------\n" << std::endl;
	}
}

static void deserializeObjects(Scene& scene, std::string line, std::map<std::string,std::shared_ptr<Shape>>& shape_map, 
	std::map<std::string, std::shared_ptr<Composite>>& composite_map, std::shared_ptr<Composite>& obj_comp){
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
					shape_map.insert(std::pair<std::string, std::shared_ptr<Shape>>(name, box));
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
					shape_map.insert(std::pair<std::string, std::shared_ptr<Shape>>(name, sphere));
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
					shape_map.insert(std::pair<std::string, std::shared_ptr<Shape>>(name, triangle));
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
					float height = std::stof(lineParts[7], NULL);
					float radius = std::stof(lineParts[8], NULL);
					std::shared_ptr<Material> mat = scene.mat_map_.at(lineParts[9]);
					std::shared_ptr<Cone> cone = std::make_shared<Cone>(base, height, radius, name, mat);
					shape_map.insert(std::pair<std::string, std::shared_ptr<Shape>>(name, cone));
					std::cout << "Cone: " << *cone << "\n";
				}
				catch (std::invalid_argument arg)
				{
					std::cout << "Something went wrong, while loading the cone. Check format!\n";
					std::cout << "Throws exception : " << arg.what() << "\n";
				}
			}
			if(lineParts[2] == "cylinder")
			{
				try{
					std::string name = lineParts[3];
					glm::vec3 base = glm::vec3{std::stof(lineParts[4],NULL),std::stof(lineParts[5],NULL),std::stof(lineParts[6],NULL)};
					float height = std::stof(lineParts[7], NULL);
					float radius = std::stof(lineParts[8],NULL);
					std::shared_ptr<Material> mat = scene.mat_map_.at(lineParts[9]);
					std::shared_ptr<Cylinder>cylinder = std::make_shared<Cylinder>(base,height,radius,name,mat);
					shape_map.insert(std::pair<std::string, std::shared_ptr<Shape>>(name, cylinder));
					std::cout<<"Cylinder: "<<*cylinder<<"\n";
				}
				catch (std::invalid_argument arg)
				{
					std::cout<<"Something went wrong, while loading the cylinder. Check format!\n";
					std::cout<<"Throws exception: "<<arg.what()<<"\n";
				}
			}
			if (lineParts[2] == "composite")
			{
				try {
					std::string name = lineParts[3];
					std::vector<std::shared_ptr<Shape>> shapes;
					std::vector<std::shared_ptr<Composite>> composites;
					for (int i = 4; i < lineParts.size(); ++i) {
						std::string current_shape = lineParts[i];
						if (shape_map.find(current_shape) != shape_map.end()) {
							shapes.push_back(shape_map.at(current_shape));
						} else if (composite_map.find(current_shape) != composite_map.end()) {
							composites.push_back(composite_map.at(current_shape));
						} else {
							std::cout << "the composite could not be added correctly";
						}
					}
					std::shared_ptr<Composite> composite = std::make_shared<Composite>(name, shapes, composites);
					composite_map.insert(std::pair<std::string, std::shared_ptr<Composite>>(name, composite));
					if (name == "root") {
						scene.root_composite_ = composite;
					}
					std::cout << "Composite: " << name << "\n\n";
				}
				catch (std::invalid_argument arg)
				{
					std::cout << "Something went wrong, while loading the ccomposite. Check format!\n";
					std::cout << "Throws exception: " << arg.what() << "\n";
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
				glm::vec3 pos = glm::vec3{ stof(lineParts[4]), stof(lineParts[5]), stof(lineParts[6]) };
				glm::vec3 dir = glm::normalize(glm::vec3{ stof(lineParts[7]), stof(lineParts[8]), stof(lineParts[9]) });
				glm::vec3 up  = glm::normalize(glm::vec3{ stof(lineParts[10]), stof(lineParts[11]), stof(lineParts[12]) });
				std::shared_ptr<Camera> camera = std::make_shared<Camera>(name, pos, dir, up , fov);
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
			readOBJ_File(path, scene, obj_comp);
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
	if (lineParts[0] == "transform") {
		std::cout << "Transform ...\n";
		try {
			std::string object = lineParts[1];
			std::shared_ptr<Shape> shape = nullptr;
			std::shared_ptr<Camera> camera = nullptr;
			if (shape_map.find(object) != shape_map.end()) {
				shape = shape_map.at(object);
			} else if (scene.camera_ != nullptr && scene.camera_->name_ == object) {
				camera = scene.camera_;
			}
			if (shape != nullptr) {
				
				if(lineParts[2] == "translate")
				{
					glm::vec3 translation{stof(lineParts[3]),stof(lineParts[4]),stof(lineParts[5])};
					shape->apply_transformation(translation,0,x_axis,glm::vec3{1.0f,1.0f,1.0f});
				}
				else if(lineParts[2] == "rotate")
				{
					float angle{stof(lineParts[3])};
					angle = (angle * 2 * M_PI) / 360;
					Axis axis;
					if(lineParts[4]=="x")
					{
						axis = x_axis;
					}
					else if(lineParts[4]=="y")
					{
						axis = y_axis;
					}
					else
					{
						axis = z_axis;
					}
					shape->apply_transformation(glm::vec3{0.0f,0.0f,0.0f},angle,axis,glm::vec3{1.0f,1.0f,1.0f});

				}
				else if(lineParts[2] == "scale")
				{
					glm::vec3 scale{stof(lineParts[3]),stof(lineParts[4]),stof(lineParts[5])};
					shape->apply_transformation(glm::vec3{0.0f,0.0f,0.0f},0,x_axis,scale);
				}


			} else if (camera != nullptr) {
				if (lineParts[2] == "rotate") {
					glm::mat4x4 rotation = glm::rotate(stof(lineParts[3]), glm::vec3{ stof(lineParts[4]), stof(lineParts[5]), stof(lineParts[6]) });
					glm::vec4 n_4 = glm::vec4{ camera->direction_, 0 } * rotation;
					glm::vec4 up_4 = glm::vec4{ camera->up_, 0 } *rotation;
					glm::vec3 n = glm::normalize(glm::vec3{ n_4.x, n_4.y , n_4.z });
					glm::vec3 up = glm::vec3{ up_4.x, up_4.y , up_4.z };
					glm::vec3 u = glm::normalize(glm::cross(n, up));
					glm::vec3 v = glm::normalize(glm::cross(u, n));
					camera->transformation_ += glm::mat4{ 
						glm::vec4{u,0.0f}, 
						glm::vec4{v,0.0f}, 
						glm::vec4{-n,0.0f}, 
						glm::vec4{0.0f} 
					};
				} else if (lineParts[2] == "translate") {
					camera->transformation_ += glm::mat4{ 
						glm::vec4{0.0f}, 
						glm::vec4{0.0f}, 
						glm::vec4{0.0f}, 
						glm::vec4{stof(lineParts[3]), stof(lineParts[4]), stof(lineParts[5]), 1.0f} };
				}
			}
		}
		catch (std::invalid_argument arg) {
			std::cout << "Something went wrong, while transforming an object or camera. Check format!\n";
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
		std::map<std::string, std::shared_ptr<Shape>> shape_map;
		std::map<std::string, std::shared_ptr<Composite>> composite_map;
		std::shared_ptr<Composite> obj_comp = nullptr;
		while(std::getline(ifs,line)){
			deserializeObjects(scene, line, shape_map, composite_map, obj_comp);
		}
		if (scene.root_composite_ != nullptr && obj_comp != nullptr) {
			scene.root_composite_->add(obj_comp);
		}
		else if (scene.root_composite_ == nullptr) {
			scene.root_composite_ = obj_comp;
		}
		scene.root_composite_->updateBoundingBox();
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