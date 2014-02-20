#include "triangle.h"
#include <math.h>
#include <iostream>

Hit Triangle::intersect(const Ray& ray){
	Vector N = (v1 - v0).cross(v2 - v0);
	double nDotRay = N.dot(ray.D);
	
	if(nDotRay > -1e-6 && nDotRay < 1e-6){
		return Hit::NO_HIT();
	}
	
	double dist = N.dot(v0);
	double t = -(N.dot(ray.O) + dist) / nDotRay;
	
	if(t < 0){
		return Hit::NO_HIT();
	}
	
	Point P = ray.O + t * ray.D;
	
	Vector C;
	
	C = (v1 - v0).cross(P - v0);
	if(N.dot(C) < 0) return Hit::NO_HIT();
	C = (v2 - v1).cross(P - v1);
	if(N.dot(C) < 0) return Hit::NO_HIT();
	C = (v0 - v2).cross(P - v2);
	if(N.dot(C) < 0) return Hit::NO_HIT();
	
	N = N.normalized();
	if(N.dot(-ray.D) < 0) N = -N;
	
	return Hit(dist, N);
}
