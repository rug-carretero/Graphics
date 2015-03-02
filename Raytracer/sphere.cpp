//
//  Framework for a raytracer
//  File: sphere.cpp
//
//  Created for the Computer Science course "Introduction Computer Graphics"
//  taught at the University of Groningen by Tobias Isenberg.
//
//  Authors:
//    Maarten Everts
//    Jasper van de Gronde
//
//  This framework is inspired by and uses code of the raytracer framework of 
//  Bert Freudenberg that can be found at
//  http://isgwww.cs.uni-magdeburg.de/graphik/lehre/cg2/projekt/rtprojekt.html 
//

#include "sphere.h"
#include <iostream>
#include <math.h>

/************************** Sphere **********************************/

Hit Sphere::intersect(const Ray &ray)
{
   Vector distance = ray.O - position;
	double a = ray.D.dot(ray.D);
	double b = distance.dot(ray.D) * 2;
	double c = distance.dot(distance) - (r * r);

	double disc = b * b - 4 * a * c;
	if (disc < 0) 
	  return Hit::NO_HIT();


	disc = sqrt(disc);

	double q;
	if (b < 0)
	  q = (-b - disc) * 0.5;
	else
	  q = (-b + disc) * 0.5;
	
	double t1 = q / a;
	double t2 = c / q;

	if (t1 > t2) {
	  double tmp = t1;
	  t1 = t2;
	  t2 = tmp;
	}

	if (t1 < 0) 
		t1 = t2;

	Vector direction = ray.D.normalized();
	
	Vector N = (ray.O + direction * t1 - position).normalized();
	
	return Hit(t1,N);
}
