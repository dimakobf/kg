#pragma once
#include "sphere.h"
#include <cmath>

class Planet
{
	float rx, ry;
	float coef = 0.175;
	float st;
	float s;

public:
	Sphere sphere;

	Planet(const float _s, const float z, const float _rx, const float _ry, const int _st, const float r, const Material& m, Image* im) : 
		s(_s), rx(_rx), ry(_ry), st(_st), sphere(Vec3f(0,0,z), r, m, im, 0)
	{

		set_current_place(0);
	}

	void set_current_place(float t) 
	{
		st += (t * s);
		float angle = (coef * (st));

		sphere.center.x = (rx * cosl(angle));
		sphere.center.y = (ry * sinl(angle));
		sphere.rotation +=s;
	}

	Vec3f current_place() {
		return sphere.center;
	}
};

class Moon
{
	float mrx, mry;
	float coef = 0.175;
	float st;
	float s;
public:

	Sphere sphere;
	Moon(Vec3f planet, const float _s, const float z, const float _rx, const float _ry, const int _st, const float r, const Material& m, Image* im) :
		s(_s), mrx(_rx), mry(_ry), st(_st), sphere(Vec3f(0, 0, z), r, m, im, 0)
	{
		set_current_place(planet, 0);
	}

	void set_current_place(Vec3f planet, float t)
	{
		st += (t * s);
		float angle = (coef * (st));

		sphere.center.x = planet.x + (mrx * cosl(angle));
		sphere.center.y = planet.y + (mry * sinl(angle));
		sphere.rotation += st;

	}

	Vec3f current_place() {
		return sphere.center;
	}

};

static std::vector<Planet> planets;
static std::vector<Moon> moons;