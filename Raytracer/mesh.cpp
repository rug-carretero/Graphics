#include "mesh.h"

Triangle * Mesh::glm2obj(GLMtriangle t){
	Point v0(model->vertices[3 * t.vindices[0] + 0], model->vertices[3 * t.vindices[0] + 1], model->vertices[3 * t.vindices[0] + 2]);
	Point v1(model->vertices[3 * t.vindices[1] + 0], model->vertices[3 * t.vindices[1] + 1], model->vertices[3 * t.vindices[1] + 2]);
	Point v2(model->vertices[3 * t.vindices[2] + 0], model->vertices[3 * t.vindices[2] + 1], model->vertices[3 * t.vindices[2] + 2]);
	
	return new Triangle(v0, v1, v2);
}

Hit Mesh::intersect(const Ray& ray){
	size_t i;
	Triangle * t;
	Hit minHit = Hit(std::numeric_limits<double>::infinity(), Vector());
	for(i = 0; i < model->numtriangles; i++){
		t = glm2obj(model->triangles[i]);
		//cout << "Mesh::intersect: " << t->v0 << ", " << t->v1 << ", " << t->v2 << endl;
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
