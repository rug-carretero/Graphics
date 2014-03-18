#include "object.h"
#include "material.h"
#include "triangle.h"
#include "glm.h"

#include <iostream>

class Mesh : public Object{
private:
	std::string file;
	GLMmodel * model;
	
	Triangle * glm2obj(GLMtriangle t);
public:
	Mesh(std::string file, float scale) : file(file), scale(scale) {
		model = glmReadOBJ(file.c_str()); 
		glmScale(model, scale); 
	}
	
	virtual Hit intersect(const Ray& ray);
	virtual Color mapTexture(const Point in);
	
	float scale;
};