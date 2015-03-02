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
#include "math.h"

int minimum = std::numeric_limits<int>::max();
int maximum = std::numeric_limits<int>::min();

bool Scene::findHit(const Ray &ray)
{
    // Find hit object and distance
    Hit min_hit(std::numeric_limits<double>::infinity(),Vector());
    
    for (unsigned int i = 0; i < objects.size(); ++i) {
        Hit hit(objects[i]->intersect(ray));

        if (hit.t<min_hit.t) {
            return true;
        }
    }
    return false;
}

Color Scene::tracePhongRec(const Ray &ray, int recursion_depth) {

    // Find hit object and distance
    Hit min_hit(std::numeric_limits<double>::infinity(),Vector());
    Object *obj = NULL;
    
    for (unsigned int i = 0; i < objects.size(); ++i) {
        Hit hit(objects[i]->intersect(ray));
       
        if (hit.t<min_hit.t) {
            min_hit = hit;
            obj = objects[i];
        }
    }

    // No hit? Return background color.
    if (!obj) return Color(0.0, 0.0, 0.0);

    Material *material = obj->material;            //the hit objects material
    Point hit = ray.at(min_hit.t);                 //the hit point
    Vector N = min_hit.N;                          //the normal at hit point
    Vector V = -ray.D;                             //the view vector

    Color ambient, diffuse, specular, i_d;
        
    double kd = material->kd;
    double ka = material->ka;
    double ks = material->ks;
    double sh = material->n;
        
    for(size_t i = 0; i < lights.size(); i++) {
        Vector L = lights[i]->position - hit;
        i_d = lights[i]->color;
    
        L.normalize();
        N.normalize();
        V.normalize();

        Vector r = ((-1 * L) + 2 * L.dot(N) * N).normalized();
        
        if (shadow_mode) {
            // Light ray from the object hit to light source
            Ray light_ray(hit+0.0001*L, L);

            // If the ray is blocked by another object, skip this light source.
            if (findHit(light_ray)) {
                ambient += ka*i_d*material->color;
                continue;
            }
        }

        ambient += ka*i_d*material->color;
        diffuse += kd*i_d*max(0.0, N.dot(L))*material->color;   
        specular += i_d * ks * pow(max(0.0, r.dot(V)), sh);
    }

    // Add reflection
    if (recursion_depth < max_recursion_depth) {
        Vector reflected_ray_direction = ray.D - 2*ray.D.dot(N)*N;
        specular += tracePhongRec(Ray(hit+0.0001*reflected_ray_direction, reflected_ray_direction), recursion_depth + 1) * ks;
    }

    return (ambient + diffuse + specular);
}

Color Scene::trace(const Ray &ray)
{

    // Find hit object and distance
    Hit min_hit(std::numeric_limits<double>::infinity(),Vector());
    Object *obj = NULL;
    
    for (unsigned int i = 0; i < objects.size(); ++i) {
        Hit hit(objects[i]->intersect(ray));
       
        if (hit.t<min_hit.t) {
            min_hit = hit;
            obj = objects[i];
        }
    }

    // No hit? Return background color.
    if (!obj) return Color(0.0, 0.0, 0.0);

    //Material *material = obj->material;            //the hit objects material
    //Point hit = ray.at(min_hit.t);                 //the hit point
    Vector N = min_hit.N;                          //the normal at hit point
    //Vector V = -ray.D;                             //the view vector
    
    if (render_mode == "phong") {
        return tracePhongRec(ray, 0);
    } else if (render_mode == "zbuffer") {
        return Color(1.0, 1.0, 1.0) - ((min_hit.t - minimum) / (maximum - minimum));
    } else if (render_mode == "normal") {
        return (N + 1)/2.0;
    }   
    return Color(0.0, 0.0, 0.0);    
}

void Scene::render(Image &img)
{
    int w = img.width();
    int h = img.height();
    
    if (render_mode == "zbuffer") {
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                Point pixel(x+0.5, h-1-y+0.5, 0);
            
                Ray ray(eye, (pixel-eye).normalized());
    
                for (unsigned int i = 0; i < objects.size(); ++i) {
                    Hit hit(objects[i]->intersect(ray));
       
                    if (hit.t < minimum) minimum = hit.t;
                    if (hit.t > maximum) maximum = hit.t;
                }               
            }
        }
    }
    
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {

            Color col = Color(0.0,0.0,0.0);
            float step = 1.0 / super_sampling;
            int i, j;

            for (i=0; i < super_sampling; i++){
                for (j=0; j < super_sampling; j++){             
                    Point pixel(x+i*step/2, h-1-y+i*step/2, 0);
                    Ray ray(eye, (pixel - eye).normalized());
                    col += trace(ray);
                }
            }

            col = col / (super_sampling*super_sampling);
            col.clamp();
            img(x, y) = col;
        }
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

void Scene::setRenderMode(std::string render_mode)
{
    this->render_mode = render_mode;
}

void Scene::setShadowMode(bool shadow_mode)
{
    this->shadow_mode = shadow_mode;
}

void Scene::setMaxRecursionDepth(int max_recursion_depth)
{
    this->max_recursion_depth = max_recursion_depth;
}

void Scene::setSuperSampling(int super_sampling)
{
    this->super_sampling = super_sampling;
}

void Scene::setCenter(Point center)
{
    this->center = center;
    this->camera = true;
}

void Scene::setUp(Point up)
{
    this->up = up;
}

void Scene::setWidthHeight(int width, int height)
{
    this->width = width;
    this->height = height;
}