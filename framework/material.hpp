#ifndef MATERIAL_STRUCT
#define MATERIAL_STRUCT
#include "color.hpp"
#include <iostream>
#include <string>
struct Material {
	Material(std::string const& name, Color ka, Color kd, Color ks, float m) : name(name), ka(ka), kd(kd), ks(ks), m(m), glossy(0), n(0), o(0) {};
	Material(std::string const& name, Color ka, Color kd, Color ks, float m, float glossy) : name(name), ka(ka), kd(kd), ks(ks), m(m), glossy(glossy), n(0), o(0) {};
	Material(std::string const& name, Color ka, Color kd, Color ks, float m, float glossy, float n, float o) : name(name), ka(ka), kd(kd), ks(ks), m(m), glossy(glossy), n(n), o(o) {};
	std::string name;
	Color ka; //ambienter reflexioskoeffizient
	Color kd; //diffuser ''
	Color ks; //reflexions '' for reflexions
	float m;
	float glossy;
	float n; //refractive index
	float o; //opacity
	friend std::ostream& operator<<(std::ostream& os, Material const& m)
	{
		os << "Name: " << m.name << ", Ka: " << m.ka << ", Kd: " << m.kd << ", Ks: " << m.ks << ", m: " << m.m << ", glossy: " << m.glossy <<", n: "<<m.n<<"\n";
		return os;
	}
};
#endif