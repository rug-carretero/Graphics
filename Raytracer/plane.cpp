#include "plane.h"
#include "image.h"
#include "material.h"
#include <math.h>

Hit Plane::intersect(const Ray& ray){
	// http://en.wikipedia.org/wiki/Line%E2%80%93plane_intersection
	Vector N = normal.normalized();
	double top = (center - ray.O).dot(N);
	double bottom = ray.D.dot(N);
	
	// denominator is zero
	if(bottom > -1e-6 && bottom < 1e-6){
		// line is either inside the plane (numerator zero)
		// or parallel and doesn't intersect (numerator non-zero)
		return Hit::NO_HIT();
	}
	
	double t = top / bottom;
	
	if(t < 0) return Hit::NO_HIT();
	
	if(N.dot(-ray.D) < 0) N = -N;
	
	return Hit(t, N);
}

Color Plane::mapTexture(const Point in){
	return material->color;
}
