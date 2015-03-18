#include "plane.h"

class Quad : public Object {
public:
	Quad(Point center, Vector normal, int radius) : center(center), normal(normal), radius(radius) {}
	
    virtual Hit intersect(const Ray& ray);
    virtual Color mapTexture(const Point in);
	
	Point center;
	Vector normal;
	int radius;
};
