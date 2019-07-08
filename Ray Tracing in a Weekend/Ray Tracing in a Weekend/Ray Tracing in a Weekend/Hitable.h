#pragma once
#ifndef HITABLEH
#define HITABLEH

#include "ray.h"

class Material;

struct hit_record
{
	float t = 0.0;
	vec3 p;
	vec3 normal;
	Material* mat_ptr = NULL;
};

class Hitable
{
	public:
		virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
};


#endif