#include "plane.h"
#include <math.h>

Hit Plane::intersect(const Ray& ray){
	// http://en.wikipedia.org/wiki/Line%E2%80%93plane_intersection
	double top = (center - ray.O).dot(normal);
	double bottom = ray.D.dot(normal);
	
	// denominator is zero
	if(bottom < 1e-6 && bottom > -1e-6){
		// line is either inside the plane (numerator zero)
		// or parallel and doesn't intersect (numerator non-zero)
		return Hit::NO_HIT();
	}
	
	double t = top / bottom;
	Vector N = normal.normalized();
	if(N.dot(-ray.D) < 0) N = -N;
	
	return Hit(t, N);
}
