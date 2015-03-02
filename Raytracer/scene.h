//
//  Framework for a raytracer
//  File: scene.h
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

#ifndef SCENE_H_KNBLQLP6
#define SCENE_H_KNBLQLP6

#include <vector>
#include "triple.h"
#include "light.h"
#include "object.h"
#include "image.h"

class Scene
{
private:
    std::vector<Object*> objects;
    std::vector<Light*> lights;
    Triple eye;
public:    
    std::string render_mode;
    bool shadow_mode;
    int max_recursion_depth;
    int super_sampling;
    Point center, up;
    int width, height;
    bool camera;
    bool findHit(const Ray &ray);
    Color tracePhongRec(const Ray &ray, int recursionDepth);
    Color trace(const Ray &ray);
    void render(Image &img);
    void addObject(Object *o);
    void addLight(Light *l);
    void setEye(Triple e);
    void setRenderMode(std::string render_mode);
    void setShadowMode(bool shadow_mode);
    void setMaxRecursionDepth(int max_recursion_depth);
    void setSuperSampling(int super_sampling);
    void setCenter(Point center);
    void setUp(Point up);
    void setWidthHeight(int width, int height);
    unsigned int getNumObjects() { return objects.size(); }
    unsigned int getNumLights() { return lights.size(); }
};

#endif /* end of include guard: SCENE_H_KNBLQLP6 */
