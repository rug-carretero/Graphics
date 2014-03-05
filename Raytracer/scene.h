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
	
	Color phongReflect(const Ray &ray, int level, Object * src);
    void phongRender(Image &img);
    void normalRender(Image &img);
    void zRender(Image &img);
    Color phongTrace(const Ray &ray, int level);
	Hit trace(const Ray& ray, Object ** obj);
public:
    void render(Image &img);
    void addObject(Object *o);
    void addLight(Light *l);
    void setEye(Triple e);
    unsigned int getNumObjects() { return objects.size(); }
    unsigned int getNumLights() { return lights.size(); }
	
	enum RenderModes {RenderPhong, RenderZBuffer, RenderNormal} renderMode;
	bool renderShadows;
	int superSamples;
	int reflectRecursion;
	
    Triple eye;
    Triple center;
    Vector up;
    int width, height;
};

#endif /* end of include guard: SCENE_H_KNBLQLP6 */
