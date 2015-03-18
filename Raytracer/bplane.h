#include "plane.h"

class BPlane : public Object {
public:
 BPlane(Point center, Vector normal, int radius, string type) : center(center), normal(normal), radius(radius), type(type) {}
	
    virtual Hit intersect(const Ray& ray);
    virtual Color mapTexture(const Point in);
	

    Point center;
    Vector normal;
    int radius;
    string type;
};
