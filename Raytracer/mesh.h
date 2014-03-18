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
	Mesh(std::string file) : file(file) {
		model = glmReadOBJ(file.c_str()); 
		glmScale(model, 100.0); 
		
		size_t i;
		for(i = 0; i < model->numvertices; i++){
			cout << model->vertices[3 * i] << ", " << model->vertices[3 * i + 1] << ", " << model->vertices[3 * i + 2] << endl;
		}
	}
	
	virtual Hit intersect(const Ray& ray);
	virtual Color mapTexture(const Point in);
};