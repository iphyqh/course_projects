#ifndef __RAYTRACER_H__
#define __RAYTRACER_H__

// The main ray tracer.

#include "scene/scene.h"
#include "scene/ray.h"

class RayTracer
{
public:
    RayTracer();
    ~RayTracer();

    vec3f trace( Scene *scene, double x, double y );
	vec3f traceRay( Scene *scene, const ray& r, const vec3f& thresh, int depth );


	void getBuffer( unsigned char *&buf, int &w, int &h );
	double aspectRatio();
	void traceSetup( int w, int h );
	void traceLines( int start = 0, int stop = 10000000 );
	void tracePixel( int i, int j );
	vec3f traceCorners(double x, double y, double sideX, double sideY, int depth, double adaptThres);
	vec3f getBackgroundColor(double u, double v);
	bool loadScene( char* fn );

	bool sceneLoaded();
	
	double getConstAtten() { return constAttenFactor; };
	bool load_background(char* fn);
private:
	unsigned char *buffer;
	int buffer_width, buffer_height;
	int bufferSize;
	double constAttenFactor;
	Scene *scene;
	unsigned char* background;
	bool m_bSceneLoaded;
	int m_nBackWidth;
	int m_nBackHeight;
};

#endif // __RAYTRACER_H__
