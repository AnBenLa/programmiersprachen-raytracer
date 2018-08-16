#ifndef AMBIENTE_STRUCT
#define AMBIENTE_STRUCT
#include "color.hpp"
struct Ambiente{
	Color color_;

	Ambiente(Color color) : color_(color){};
	
	friend std::ostream& operator<<(std::ostream& os, Ambiente const& m)
	{
		os << m.color_ << "\n";
		return os;
	}
};
#endif