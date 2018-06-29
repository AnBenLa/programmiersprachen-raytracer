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
	std::ostream& operator <<(std::ostream& os) {
		return os << "\n Farbe: \n Name: " << name << " , Ka: " << ka << " , Kd: " << kd << " , Ks: " << ks << " , m: " << m << "\n";
	}
};
#endif