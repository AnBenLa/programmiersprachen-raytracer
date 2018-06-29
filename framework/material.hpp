#ifndef MATERIAL_STRUCT
#define MATERIAL_STRUCT
#include "color.hpp"
#include <iostream>
#include <string>
struct Material {
	Material(std::string name, Color ka, Color kd, Color ks, float m) : name(name), ka(ka), kd(kd), ks(ks), m(m) {};
	std::string name;
	Color ka;
	Color kd;
	Color ks;
	float m;
	friend std::ostream& operator<<(std::ostream& os, Material const& m)
	{
		os << "\n Name: " << m.name << " , Ka: " << m.ka << " , Kd: " << m.kd << " , Ks: " << m.ks << " , m: " << m.m << "\n";
		return os;
	}
};
#endif