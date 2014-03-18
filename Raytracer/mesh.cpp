#include "mesh.h"

Triangle * Mesh::glm2obj(GLMtriangle t){
	Point v0 = Point(model->vertices[t.vindices[0]]);
	Point v1 = Point(model->vertices[t.vindices[1]]);
	Point v2 = Point(model->vertices[t.vindices[2]]);
	
	return new Triangle(v0, v1, v2);
}

Hit Mesh::intersect(const Ray& ray){
	size_t i;
	Triangle * t;
	Hit minHit = Hit(std::numeric_limits<double>::infinity(), Vector());
	for(i = 0; i < model->numtriangles; i++){
		t = glm2obj(model->triangles[i]);
		Hit h(t->intersect(ray));
		if(h.t < minHit.t){
			minHit = h;
		}
	}
	
	return minHit;
}

Color Mesh::mapTexture(const Point in){
	return material->color;
}
