#include "hit.h"
#include "bplane.h"
#include "material.h"

#include <math.h>

/*
 It's just a plane with boundaries
*/
Hit BPlane::intersect(const Ray& ray){
	Vector N = normal.normalized();
	double top = (center - ray.O).dot(N);
	double bottom = ray.D.dot(N);
	
	// denominator is zero
	if(bottom > -1e-6 && bottom < 1e-6){
		return Hit::NO_HIT();
	}
	
	double t = top / bottom;
	
	if(t < 0) return Hit::NO_HIT();
	
	if(N.dot(-ray.D) < 0) N = -N;
	
	Point p = ray.at(t);

	

	
	if(type == "quad" && fabs(p.x - center.x) <= radius && fabs(p.y - center.y) <= radius && fabs(p.z - center.z) <= radius) return Hit(t, N);
	if(type == "circle" && (p-center).length() <= radius) return Hit(t, N);
	return Hit::NO_HIT();
	

}

Color BPlane::mapTexture(const Point in){
	return material->color;
}
