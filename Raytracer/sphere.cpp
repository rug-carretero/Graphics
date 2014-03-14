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
#include "image.h"
#include "material.h"
#include <iostream>
#include <math.h>

/************************** Sphere **********************************/

Hit Sphere::intersect(const Ray &ray)
{
    /****************************************************
    * RT1.1: INTERSECTION CALCULATION
    *
    * Given: ray, position, r
    * Sought: intersects? if true: *t
    * 
    * Insert calculation of ray/sphere intersection here. 
    *
    * You have the sphere's center (C) and radius (r) as well as
    * the ray's origin (ray.O) and direction (ray.D).
    *
    * If the ray does not intersect the sphere, return false.
    * Otherwise, return true and place the distance of the
    * intersection point from the ray origin in *t (see example).
    ****************************************************/
	/*
		zie ook
		http://en.wikipedia.org/wiki/Line%E2%80%93sphere_intersection
	*/

	Vector distance = ray.O - position;
	Vector direction = ray.D.normalized();
	
	double a = direction.dot(direction);
	double b = 2 * (direction.dot(distance));
	double c = distance.dot(distance) - r * r;
	double discriminant = b * b - 4 * a * c;
	
	if(discriminant < 0) return Hit::NO_HIT();
	
	double intersect1 = (-b + sqrt(discriminant)) / (2 * a);
	double intersect2 = (-b - sqrt(discriminant)) / (2 * a);
	
	if(intersect1 < 0 && intersect2 < 0){
		return Hit::NO_HIT();
	}
	
	double t = 0;
	
	if(intersect1 > 0 && intersect2 > 0){
		t = min(intersect1, intersect2);
	}else{
		t = max(intersect1, intersect2);
	}
	
	/****************************************************
	* RT1.2: NORMAL CALCULATION
	*
	* Given: t, C, r
	* Sought: N
	* 
	* Insert calculation of the sphere's normal at the intersection point.
	****************************************************/
	
	Vector N = (ray.O + direction * t - position).normalized();
	// if(distance.length() < r) N *= -1;
	if(N.dot(-ray.D) < 0) N = -N;
	
	//Vector N = (ray.O + direction.normalized() * t - position).normalized();
	
	return Hit(t,N);
}

Color Sphere::mapTexture(const Point in){
	Image * texture = material->texture;
	
	if(!texture) return material->color;
	
	Vector vn = Vector(0, 1, 0).normalized();
	Vector ve = Vector(1, 0, 0);
	Vector vp = (in - position).normalized();
	
	double phi = acos(-vn.dot(vp));
	double v = 1 - (phi / M_PI);
	double theta = (acos(vp.dot(ve)) / sin(phi)) / (2.0 * M_PI);
	double u = 1 - theta;
	
	if(vn.cross(ve).dot(vp) > 0 ) u = theta;
	
	return texture->colorAt(u, v);
}
