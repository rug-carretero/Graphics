#include "triangle.h"
#include "image.h"
#include "material.h"
#include <math.h>
#include <iostream>

Hit Triangle::intersect(const Ray& ray){
	Vector N = (v1 - v0).cross(v2 - v0);
	// double nDotRay = N.dot(ray.D);
	
	// if(nDotRay > -1e-6 && nDotRay < 1e-6){
	// 	return Hit::NO_HIT();
	// }
	
	// double dist = N.dot(v0);
	// double t = -(N.dot(ray.O) + dist) / nDotRay;
	
	// if(t < 0){
	// 	return Hit::NO_HIT();
	// }
	
	// Point P = ray.O + t * ray.D;
	
	// Vector C;
	
	// C = (v1 - v0).cross(P - v0);
	// if(N.dot(C) < 0) return Hit::NO_HIT();
	// C = (v2 - v1).cross(P - v1);
	// if(N.dot(C) < 0) return Hit::NO_HIT();
	// C = (v0 - v2).cross(P - v2);
	// if(N.dot(C) < 0) return Hit::NO_HIT();
	
	// N = N.normalized();
	// if(N.dot(-ray.D) < 0) N = -N;
	
	// return Hit(dist, N);

#define EPSILON 0.000001
 
	/*int triangle_intersection( const Vec3   V1,  // Triangle vertices
                           const Vec3   V2,
                           const Vec3   V3,
                           const Vec3    O,  //Ray origin
                           const Vec3    D,  //Ray direction
                                 float* out )
				 {*/
  Vector e1, e2;  //Edge1, Edge2
  Vector P, Q, T;
  float det, inv_det, u, v;
  float t;
 
  //Find vectors for two edges sharing V1
  //SUB(e1, V2, V1);
  e1 = v1 - v0;
  //SUB(e2, V3, V1);
  e2 =  v2 - v0;


 //Begin calculating determinant - also used to calculate u parameter
  //CROSS(P, D, e2);
  P = ray.D.cross(e2);


  //if determinant is near zero, ray lies in plane of triangle
  det = e1.dot(P);
  //NOT CULLING
  if(det > -EPSILON && det < EPSILON) return Hit::NO_HIT();
  inv_det = 1.f / det;
 
  //calculate distance from V1 to ray origin
  //SUB(T, O, V1);
  T = ray.O - v0;
 
  //Calculate u parameter and test bound
  u = T.dot(P) * inv_det;
  //The intersection lies outside of the triangle
  if(u < 0.f || u > 1.f) return Hit::NO_HIT();
 
  //Prepare to test v parameter
  //CROSS(Q, T, e1);
  Q = T.cross(e1);
 
  //Calculate V parameter and test bound
  v = ray.D.dot(Q) * inv_det;

  //The intersection lies outside of the triangle
  if(v < 0.f || u + v  > 1.f) return Hit::NO_HIT();
 
  t = e2.dot(Q) * inv_det;
 
  if(t > EPSILON) { //ray intersection
    //*out = t;
    return Hit(t,N);
  }
 
  // No hit, no win
  return Hit::NO_HIT();


}

Color Triangle::mapTexture(const Point in){
	return material->color;
}
