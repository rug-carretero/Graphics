#include "object.h"
#include "material.h"
#include "triangle.h"
#include "glm.h"

class Mesh : public Object{
private:
	std::string file;
	GLMmodel * model;
	
	Triangle * glm2obj(GLMtriangle t);
public:
	Mesh(std::string file) : file(file) { model = glmReadOBJ(file.c_str()); }
	
	virtual Hit intersect(const Ray& ray);
	virtual Color mapTexture(const Point in);
};