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

    return min_hit;
}

Color Scene::phongTrace(const Ray &ray)
{
    Object * obj;
    
    Hit min_hit = trace(ray, &obj);
    
    if(!obj) return Vector(0.0, 0.0, 0.0);

    Material *material = obj->material;            //the hit objects material
    Point hit = ray.at(min_hit.t);                 //the hit point
    Vector N = min_hit.N.normalized();             //the normal at hit point
    Vector V = -ray.D.normalized();                //the view vector


    /****************************************************
    * This is where you should insert the color
    * calculation (Phong model).
    *
    * Given: material, hit, N, V, lights[]
    * Sought: color
    *
    * Hints: (see triple.h)
    *        Triple.dot(Vector) dot product
    *        Vector+Vector      vector sum
    *        Vector-Vector      vector difference
    *        Point-Point        yields vector
    *        Vector.normalize() normalizes vector, returns length
    *        double*Color        scales each color component (r,g,b)
    *        Color*Color        dito
    *        pow(a,b)           a to the power of b
    ****************************************************/

    //see also http://en.wikipedia.org/wiki/Phong_reflection_model

    Color color = material->color;

    Color Il = Color(0,0,0);

	Color specular = Color(0,0,0);

    for (size_t i = 0; i < lights.size(); i++){
		
		Vector Lm = (lights[i]->position - hit).normalized();
		Vector Rm = 2* Lm.dot(N) * N - Lm;
		
		/* ambient */
		Il += material->ka * lights[i]->color;
		
		if(renderShadows){
			Object * hobj = NULL;
			trace(Ray(hit, -Lm), &hobj);
			if(hobj != obj) continue; // light-ray hits object: shadow
		}
		
		/* diffuse */
		double diffuse = material->kd * max(0.0,Lm.dot(N));
		Il +=  diffuse * lights[i]->color;
		
		/* specular */
	    specular += material->ks * pow(max(0.0,Rm.dot(V)),material->n) * lights[i]->color;
    }

    return color * Il + specular;
}

void Scene::zRender(Image &img){
    int w = img.width();
    int h = img.height();

    double min = std::numeric_limits<double>::infinity();
    double max = 0.0;

	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			Point pixel(x+0.5, h-1-y+0.5, 0);
			Ray ray(eye, (pixel-eye).normalized());
			
			double dist = trace(ray, NULL).t;
			
			if (dist >= 0){
				if(dist < min) min = dist;
				if(dist > max) max = dist;
			}
		}
	}

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            Point pixel(x+0.5, h-1-y+0.5, 0);
            Ray ray(eye, (pixel-eye).normalized());
            double dist = trace(ray, NULL).t;
            dist = dist < 0 ? 0 : 1.0 - ((dist - min) / (max - min));
            Color col = Color(dist,dist,dist);
            img(x,y) = col;
        }
    }
}

void Scene::phongRender(Image &img)
{
    int w = img.width();
    int h = img.height();
	double sqrtSamples = 1.0/sqrt((double)superSamples);
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
			Color col = Color(0.0, 0.0, 0.0);
			double xx = x + sqrtSamples * .5;
			double yy = h - y - sqrtSamples * .5;
			for(int aax = 0; aax*aax < superSamples; aax++){
				for(int aay = 0; aay*aay < superSamples; aay++){
					Point pixel(xx + sqrtSamples*aax, yy - sqrtSamples*aay, 0);
					Ray ray(eye, (pixel-eye).normalized());
					col += phongTrace(ray) / (double)superSamples;
				}
			}
			(col).clamp();
			img(x,y) = col;
        }
    }
}

void Scene::normalRender(Image &img){
	int w = img.width();
    int h = img.height();
	Vector right = (center - eye).cross(up).normalized()*up.length();
	cout << "eye: " << eye << ", center: " << center << ", up: " << up << ", right: " << right << ", in: " << (center - eye) << endl;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            // Point pixel(x+.5 + center.x - (double)w/2.0, h-1-y+.5 + center.y - (double)h/2.0, 0);
			Point pixel(x+.5, h-1-y+.5, 0);
			pixel += center - right*(double)width/2.0 - up*(double)height/2.0;
			Ray ray(eye, (pixel - eye).normalized());
            //Color col = normalTrace(ray);
            Color col = (trace(ray, NULL).N.normalized() + 1.0)/2.0;
            col.clamp();
            img(x,y) = col;
        }
    }
}

void Scene::render(Image &img){
	switch(renderMode){
		case RenderPhong: phongRender(img); break;
		case RenderZBuffer: zRender(img); break;
		case RenderNormal: normalRender(img); break;
	}
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
