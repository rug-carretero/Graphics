//
//  Framework for a raytracer
//  File: scene.cpp
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

#include "scene.h"
#include "material.h"

#include <iostream>

Hit Scene::trace(const Ray& ray, Object ** object){
	// Find hit object and distance
    Hit min_hit(std::numeric_limits<double>::infinity(),Vector());
    Object *obj = NULL;
    for (unsigned int i = 0; i < objects.size(); ++i) {
        Hit hit(objects[i]->intersect(ray));
        if (hit.t < min_hit.t) {
            min_hit = hit;
            obj = objects[i];
        }
    }

    // No hit? Return negative.
    if (!obj){
		if(object) *object = NULL;
		return Hit::NO_HIT();
	}
    
    if(object){
		*object = obj;
	}
	
	if(min_hit.t < 0) cout << "Min-hit..? " << min_hit.t << endl;

    return min_hit;
}

Color Scene::phongTrace(const Ray &ray, int level)
{
	Object * obj;
		
	Hit min_hit = trace(ray, &obj);
		
	if(!obj) return Vector(0.0, 0.0, 0.0);

	Material *material = obj->material;						//the hit objects material
	Point hit = ray.at(min_hit.t);								 //the hit point
	Vector N = min_hit.N.normalized();						 //the normal at hit point
	Vector V = -ray.D.normalized();								//the view vector


	/****************************************************
	 * This is where you should insert the color
	 * calculation (Phong model).
	 *
	 * Given: material, hit, N, V, lights[]
	 * Sought: color
	 *
	 * Hints: (see triple.h)
	 *				Triple.dot(Vector) dot product
	 *				Vector+Vector			vector sum
	 *				Vector-Vector			vector difference
	 *				Point-Point				yields vector
	 *				Vector.normalize() normalizes vector, returns length
	 *				double*Color				scales each color component (r,g,b)
	 *				Color*Color				dito
	 *				pow(a,b)					 a to the power of b
	 ****************************************************/

	//see also http://en.wikipedia.org/wiki/Phong_reflection_model

	Color color = obj->mapTexture(hit);

	Color Il = Color(0,0,0);

	Color specular = Color(0,0,0);

	for (size_t i = 0; i < lights.size(); i++){
		
		Vector Lm = (lights[i]->position - hit).normalized();
		Vector Rm = 2* Lm.dot(N) * N - Lm;
		
		/* ambient */
		Il += material->ka * lights[i]->color;
	
		if(renderShadows){
			Object * hobj = NULL;
			Hit h = trace(Ray(lights[i]->position, -Lm), &hobj);
			if(h.t < (lights[i]->position - hit).length() - 0.01){
				//cout << "t: " << min_hit.t << ", h-t: " << h.t << endl;
				continue; // light-ray hits object: shadow
			}
		}
	
		/* diffuse */
		double diffuse = material->kd * max(0.0,Lm.dot(N));
		Il += diffuse * lights[i]->color;
		
		/* specular */
		specular += material->ks * pow(max(0.0, Rm.dot(V)), material->n) * lights[i]->color;
	}
	Color col = color * Il + specular;
	
	if(level < reflectRecursion){
		Vector r = - V + 2 * V.dot(N) * N;
		col += material->ks * phongTrace(Ray(hit + r.normalized(), r), level + 1);
	}
	
	return col;
}

void Scene::zRender(Image &img){
    int w = img.width();
    int h = img.height();

    zmin = std::numeric_limits<double>::infinity();
    zmax = 0.0;

	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			Point pixel(x+0.5, h-1-y+0.5, 0);
			Ray ray(eye, (pixel-eye).normalized());
			
			double dist = trace(ray, NULL).t;
			
			if (dist >= 0){
				if(dist < zmin) zmin = dist;
				if(dist > zmax) zmax = dist;
			}
		}
	}
}

Color Scene::goochTrace(const Ray& ray){
	Object * obj;
	
	Hit min_hit = trace(ray, &obj);
	
	if(!obj) return Vector(0.0, 0.0, 0.0);

	Material *material = obj->material;						//the hit objects material
	Point hit = ray.at(min_hit.t);								 //the hit point
	Vector N = min_hit.N.normalized();						 //the normal at hit point
	Vector V = -ray.D.normalized();								//the view vector

	Color Il = Color(0,0,0);

	Color specular = Color(0,0,0);

	for (size_t i = 0; i < lights.size(); i++){
		
		Vector Lm = (lights[i]->position - hit).normalized();
		Vector Rm = 2* Lm.dot(N) * N - Lm;
		
		Color kd = lights[i]->color * material->color * material->kd;
		
		Color kCool = Color(0.0, 0.0, goochB) + alpha * kd;
		Color kWarm = Color(goochY, goochY, 0.0) + beta * kd;
		Il += kCool *(1.0 - N.dot(Lm))/2.0 + kWarm * (1 + N.dot(Lm))/2.0;
		 
		/* specular */
		specular += material->ks * pow(max(0.0, Rm.dot(V)), material->n) * lights[i]->color;
	}
	Color col = Il + specular;
	
	return col;
}

void Scene::phongRender(Image &img)
{
    int w = img.width();
    int h = img.height();
    Vector right = (center - eye).normalized().cross(up).normalized()*up.length();
	double sqrtSamples = 1.0/sqrt((double)superSamples);
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
			Color col = Color(0.0, 0.0, 0.0);
			double xx = x + sqrtSamples * .5;
			double yy = h - y - sqrtSamples * .5;
			for(int aax = 0; aax*aax < superSamples; aax++){
				for(int aay = 0; aay*aay < superSamples; aay++){
					Point pixel(xx + sqrtSamples*aax, yy - sqrtSamples*aay, 0);
					
					pixel = center + (pixel.x - (double)width/2.0)*right + (pixel.y - (double)height/2.0)*up;
					
					Ray ray(eye, (pixel-eye).normalized());
					// col += phongTrace(ray, 0) / (double)superSamples;
					Color cccol;
					double dist;
					switch(renderMode){
						case RenderPhong: cccol = phongTrace(ray, 0); break;
						case RenderNormal: cccol = (trace(ray, NULL).N.normalized() + 1.0)/2.0; break;
						case RenderZBuffer: 
							dist = trace(ray, NULL).t;
							dist = dist < 0 ? 0 : 1.0 - ((dist - zmin) / (zmax - zmin));
							cccol = Color(dist,dist,dist);
						break;
						case RenderGooch: cccol = goochTrace(ray); break;
					}
					col += cccol / (double)superSamples;
				}
			}
			(col).clamp();
			img(x,y) = col;
        }
    }
}

void Scene::render(Image &img){
	if (renderMode == RenderZBuffer) zRender(img);
	phongRender(img);
}

void Scene::addObject(Object *o)
{
    objects.push_back(o);
}

void Scene::addLight(Light *l)
{
    lights.push_back(l);
}

void Scene::setEye(Triple e)
{
    eye = e;
}
