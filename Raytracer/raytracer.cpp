//
//  Framework for a raytracer
//  File: raytracer.cpp
//
//  Created for the Computer Science course "Introduction Computer Graphics"
//  taught at the University of Groningen by Tobias Isenberg.
//
//  Author: Maarten Everts
//
//  This framework is inspired by and uses code of the raytracer framework of 
//  Bert Freudenberg that can be found at
//  http://isgwww.cs.uni-magdeburg.de/graphik/lehre/cg2/projekt/rtprojekt.html 
//

#include "raytracer.h"
#include "object.h"
#include "sphere.h"
#include "plane.h"
#include "triangle.h"
#include "material.h"
#include "light.h"
#include "image.h"
#include "yaml/yaml.h"
#include <ctype.h>
#include <fstream>
#include <assert.h>

// Functions to ease reading from YAML input
void operator >> (const YAML::Node& node, Triple& t);
Triple parseTriple(const YAML::Node& node);

void operator >> (const YAML::Node& node, Triple& t)
{
    assert(node.size()==3);
    node[0] >> t.x;
    node[1] >> t.y;
    node[2] >> t.z;
}

Triple parseTriple(const YAML::Node& node)
{
    Triple t;
    node[0] >> t.x;
    node[1] >> t.y;
    node[2] >> t.z;	
    return t;
}

Material* Raytracer::parseMaterial(const YAML::Node& node)
{
    Material *m = new Material();
    
    if(const YAML::Node * texnode = node.FindValue("texture")){
		std::string file;
		*texnode >> file;
		
		m->loadTexture(file);
		
		cout << "Texture: " << m->texture->width() << "x" << m->texture->height() << endl;
	}else{
		node["color"] >> m->color;	
		m->texture = NULL;
	}
	
    node["ka"] >> m->ka;
    node["kd"] >> m->kd;
    node["ks"] >> m->ks;
    node["n"] >> m->n;
    
    return m;
}

Object* Raytracer::parseObject(const YAML::Node& node)
{
    Object *returnObject = NULL;
    std::string objectType;
    node["type"] >> objectType;

    if (objectType == "sphere") {
        Point pos;
        node["position"] >> pos;
        double r;
        node["radius"] >> r;
        Sphere *sphere = new Sphere(pos,r);		
        returnObject = sphere;
    }
    
    if(objectType == "plane"){
		Point center;
		node["center"] >> center;
		Vector normal;
		node["normal"] >> normal;
		Plane * plane = new Plane(center, normal);
		returnObject = plane;
	}
	
	if(objectType == "triangle"){
		Point v0, v1, v2;
		node["v0"] >> v0;
		node["v1"] >> v1;
		node["v2"] >> v2;
		Triangle * triangle = new Triangle(v0, v1, v2);
		returnObject = triangle;
	}

    if (returnObject) {
        // read the material and attach to object
        returnObject->material = parseMaterial(node["material"]);
    }

    return returnObject;
}

Light* Raytracer::parseLight(const YAML::Node& node)
{
    Point position;
    node["position"] >> position;
    Color color;
    node["color"] >> color;
    return new Light(position,color);
}

enum Scene::RenderModes parseRenderMode(const YAML::Node& node){
	if(node == "zbuffer") return Scene::RenderZBuffer;
	if(node == "normal") return Scene::RenderNormal;
	if(node == "gooch") return Scene::RenderGooch;
	return Scene::RenderPhong;
}

bool parseBool(const YAML::Node& node){
	string val;
	node >> val;
	
	for(size_t i = 0; i < val.length(); i++){
		val[i] = tolower(val[i]);
	}
	
	if(val == "true" || val == "yes" || val == "1") return true;
	return false;
}

/*
* Read a scene from file
*/

bool Raytracer::readScene(const std::string& inputFilename)
{
    // Initialize a new scene
    scene = new Scene();

    // Open file stream for reading and have the YAML module parse it
    std::ifstream fin(inputFilename.c_str());
    if (!fin) {
        cerr << "Error: unable to open " << inputFilename << " for reading." << endl;;
        return false;
    }
    try {
        YAML::Parser parser(fin);
        if (parser) {
            YAML::Node doc;
            parser.GetNextDocument(doc);
			
			scene->renderMode = Scene::RenderPhong;
            // Read scene configuration options
			if(const YAML::Node * rMode = doc.FindValue("RenderMode")){
				scene->renderMode = parseRenderMode(*rMode);
			}
			if(const YAML::Node * rShadows = doc.FindValue("Shadows")){
				scene->renderShadows = parseBool(*rShadows);
			}
			const YAML::Node * ssing = doc.FindValue("SuperSampling");
            if(ssing) {
				cout << "Super-sampling: ";
                (*ssing)["factor"] >> scene->superSamples;
				cout << scene->superSamples << endl;
            }else{
				scene->superSamples = 1;
			}
			
			const YAML::Node * camera = doc.FindValue("Camera");
			if(camera){
				(*camera)["eye"] >> scene->eye;
				(*camera)["center"] >> scene->center;
				(*camera)["up"] >> scene->up;
				(*camera)["viewSize"][0] >> scene->width;
				(*camera)["viewSize"][1] >> scene->height;
			}else{
				scene->setEye(parseTriple(doc["Eye"]));
				scene->center = Triple(0, 0, 0);
				scene->up = Vector(0, 1, 0);
				scene->width = 400;
				scene->height = 400;
			}
			
			const YAML::Node * refRec = doc.FindValue("MaxRecursionDepth");
			if(refRec){
				*refRec >> scene->reflectRecursion;
			}else{
				scene->reflectRecursion = 0;
			}

            // Read and parse the scene objects
            const YAML::Node& sceneObjects = doc["Objects"];
            if (sceneObjects.GetType() != YAML::CT_SEQUENCE) {
                cerr << "Error: expected a sequence of objects." << endl;
                return false;
            }
            for(YAML::Iterator it=sceneObjects.begin();it!=sceneObjects.end();++it) {
                Object *obj = parseObject(*it);
                // Only add object if it is recognized
                if (obj) {
                    scene->addObject(obj);
                } else {
                    cerr << "Warning: found object of unknown type, ignored." << endl;
                }
            }

            // Read and parse light definitions
            const YAML::Node& sceneLights = doc["Lights"];
            if (sceneObjects.GetType() != YAML::CT_SEQUENCE) {
                cerr << "Error: expected a sequence of lights." << endl;
                return false;
            }
            for(YAML::Iterator it=sceneLights.begin();it!=sceneLights.end();++it) {
                scene->addLight(parseLight(*it));
            }
        }
        if (parser) {
            cerr << "Warning: unexpected YAML document, ignored." << endl;
        }
    } catch(YAML::ParserException& e) {
        std::cerr << "Error at line " << e.mark.line + 1 << ", col " << e.mark.column + 1 << ": " << e.msg << std::endl;
        return false;
    }

    cout << "YAML parsing results: " << scene->getNumObjects() << " objects read." << endl;
    return true;
}

void Raytracer::renderToFile(const std::string& outputFilename)
{
    Image img(scene->width,scene->height);
    cout << "Tracing..." << endl;
	cout << "Render mode: ";// << scene->renderMode;
	switch(scene->renderMode){
		case Scene::RenderPhong: cout << " phong"; break;
		case Scene::RenderZBuffer: cout << " zbuffer"; break;
		case Scene::RenderNormal: cout << " normal"; break;
	}
	cout << endl;
	cout << "Reflection recursion: " << scene->reflectRecursion << endl;
	
    scene->render(img);
    
    cout << "Writing image to " << outputFilename << "..." << endl;
    img.write_png(outputFilename.c_str());
    cout << "Done." << endl;
}
