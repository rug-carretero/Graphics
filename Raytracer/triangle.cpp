#include "triangle.h"
#include "image.h"
#include "material.h"
#include <math.h>
#include <iostream>

Hit Triangle::intersect(const Ray& ray){

  //source: http://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm

#define EPSILON 0.000001
 
  Vector e1, e2;  //Edge1, Edge2
  Vector P, Q, T, N;
  float det, inv_det, u, v;
  float t;
 
  e1 = v1 - v0;
  e2 =  v2 - v0;

  N = e1.cross(e2);

  P = ray.D.cross(e2);


  det = e1.dot(P);
  if(det > -std::numeric_limits<double>::epsilon() && det < std::numeric_limits<double>::epsilon()) return Hit::NO_HIT();
  inv_det = 1.f / det;
 
  T = ray.O - v0;
 
  u = T.dot(P) * inv_det;
  if(u < 0.f || u > 1.f) return Hit::NO_HIT();
 
  Q = T.cross(e1);
 
  v = ray.D.dot(Q) * inv_det;

  if(v < 0.f || u + v  > 1.f) return Hit::NO_HIT();
 
  t = e2.dot(Q) * inv_det;
 
  if(t > std::numeric_limits<double>::epsilon()) { //ray intersection
    //*out = t;
    return Hit(t,N);
  }
 
  // No hit, no win
  return Hit::NO_HIT();

}

Color Triangle::mapTexture(const Point in){
	return material->color;
}
