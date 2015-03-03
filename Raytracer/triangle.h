#pragma once

#include "object.h"

class Triangle : public Object{
public:
	Triangle(Point v0, Point v1, Point v2) : v0(v0), v1(v1), v2(v2) {}
	
	virtual Hit intersect(const Ray& ray);
	virtual Color mapTexture(const Point in);
	
	const Point v0;
	const Point v1;
	const Point v2;
};
