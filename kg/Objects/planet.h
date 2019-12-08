#pragma once
#include "sphere.h"
#include <cmath>

class Planet : public Sphere
{
	//Vec3f center; на случай, если решу не 0;0;0
	//Vec3f point;
	//float speed1, speed2;
	float rx, ry;
	float coef = 0.175;
	float st;
	float s;

public:
	Planet(const float _s, const float z, const float _rx, const float _ry, const int _st, const float r, const Material& m) : 
		s(_s), rx(_rx), ry(_ry), st(_st), Sphere(Vec3f(0,0,z), r, m)
	{

		set_current_place(0);
	}

	void set_current_place(float t) 
	{
		st += (t * s);
		float angle = (coef * (st));

		center.x = (rx * cosl(angle));
		center.y = (ry * sinl(angle));

	}

	Vec3f current_place() {
		return center;
	}
};

static std::vector<Planet> planets;