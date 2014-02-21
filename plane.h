#pragma once

#include "object.h"

class Plane : public Object{
public:
	Plane(Point center, Vector normal) : center(center), normal(normal) {}
	
	virtual Hit intersect(const Ray& ray);
	
	const Point center;
	const Vector normal;
};
