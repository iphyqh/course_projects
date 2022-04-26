// The main ray tracer.

#include <Fl/fl_ask.h>

#include "fileio/bitmap.h"
#include "RayTracer.h"
#include "scene/light.h"
#include "scene/material.h"
#include "scene/ray.h"
#include "fileio/read.h"
#include "fileio/parse.h"
#include "ui/TraceUI.h"
extern TraceUI* traceUI;
// Trace a top-level ray through normalized window coordinates (x,y)
// through the projection plane, and out into the scene.  All we do is
// enter the main ray-tracing method, getting things started by plugging
// in an initial ray weight of (0.0,0.0,0.0) and an initial recursion depth of 0.
vec3f RayTracer::trace( Scene *scene, double x, double y )
{
    ray r( vec3f(0,0,0), vec3f(0,0,0) );
    scene->getCamera()->rayThrough( x,y,r );
	return traceRay( scene, r, vec3f(1.0,1.0,1.0), 0 ).clamp();
}

// Do recursive ray tracing!  You'll want to insert a lot of code here
// (or places called from here) to handle reflection, refraction, etc etc.
vec3f RayTracer::traceRay( Scene *scene, const ray& r, 
	const vec3f& thresh, int depth )
{
	isect i;
	
	if (depth == traceUI->getDepth()) // Stop recursion when depth limit is met
		return vec3f(0.0, 0.0, 0.0);

	if (scene->intersect(r, i)) {
		// YOUR CODE HERE

		// An intersection occured!  We've got work to do.  For now,
		// this code gets the material for the surface that was intersected,
		// and asks that material to provide a color for the ray.  

		// This is a great place to insert code for recursive ray tracing.
		// Instead of just returning the result of shade(), add some
		// more steps: add in the contributions from reflected and refracted
		// rays.

		const Material& m = i.getMaterial();
		vec3f I = m.shade(scene, r, i);

		if (depth > 1 && I.length() <  10 * traceUI->getThreshold())
			return vec3f(0.0,0.0,0.0);

		// Handle reflected ray as primary ray, only if reflection property is specified, so that identical effect as that of sample .exe
		/*if (i.getMaterial().ks != vec3f(0.0,0.0,0.0)) {*/
		vec3f N;
		if (i.N.dot(r.getDirection()) < 0)
			N = i.N.normalize();
		else
			N = -i.N.normalize();
		vec3f L = r.getDirection().normalize();
		vec3f R = (N.dot(L) * 2 * N - L).normalize();
		ray reflect = ray(r.at(i.t) - RAY_EPSILON * R, -R);
		/*ray reflect = ray(r.at(i.t) - RAY_EPSILON * (i.N.dot(r.getDirection()) * 2 * i.N - r.getDirection()), -(i.N.dot(r.getDirection()) * 2 * i.N - r.getDirection()));*/

		I += prod(i.getMaterial().kr, traceRay(scene, reflect, thresh, depth + 1).clamp());
		/*}*/

		// refracted ray
		int negN = 1;
		double n_i;//old
		double n_t;//new

		double cos_i = (i.N.normalize().dot(r.getDirection().normalize()));
		
		if (i.N.dot(r.getDirection()) < 0) {
			n_i = 1;
			n_t = m.index;
			cos_i = -cos_i;
		}
		else
		{
			n_i = m.index;
			n_t = 1;
			negN = -1; // Reverse N direction
		}

		

		double sin_i = sqrt(1 - cos_i * cos_i);
		double sin_t = sin_i * n_i / n_t;

		if (sin_t < 1) {
			double cos_t = sqrt(1 - sin_t * sin_t);
			//vec3f N = normalVec.normalize();
			//vec3f M = (cos_t * N - I) / sin_i;
			vec3f T = (n_i / n_t * cos_i - cos_t) * negN*i.N.normalize() + n_i / n_t * r.getDirection().normalize();
			ray next_r = ray(r.at(i.t) + T * RAY_EPSILON, T);
			I += prod(m.kt, traceRay(scene, next_r, thresh, depth + 1));
		}


		return I;
	
	} else {
		// No intersection.  This ray travels to infinity, so we color
		// it according to the background color, which in this (simple) case
		// is just black.
		double z = r.getDirection()[2];
		double y = r.getDirection()[1];
		double x= r.getDirection()[0];
		double newx = x / z + 0.5;
		double newy = y / z + 0.5;

		if (traceUI->useBackground() && background) return getBackgroundColor(newx, newy);
		
		return vec3f( 0.0, 0.0, 0.0 );
	}
}

RayTracer::RayTracer()
{
	buffer = NULL;
	buffer_width = buffer_height = 256;
	scene = NULL;
	background = NULL;
	m_bSceneLoaded = false;
}


RayTracer::~RayTracer()
{
	delete [] buffer;
	delete scene;
}

void RayTracer::getBuffer( unsigned char *&buf, int &w, int &h )
{
	buf = buffer;
	w = buffer_width;
	h = buffer_height;
}

double RayTracer::aspectRatio()
{
	return scene ? scene->getCamera()->getAspectRatio() : 1;
}

bool RayTracer::sceneLoaded()
{
	return m_bSceneLoaded;
}

bool RayTracer::loadScene( char* fn )
{
	try
	{
		scene = readScene( fn );
	}
	catch( ParseError pe )
	{
		fl_alert( "ParseError: %s\n", pe );
		return false;
	}

	if( !scene )
		return false;
	
	buffer_width = 256;
	buffer_height = (int)(buffer_width / scene->getCamera()->getAspectRatio() + 0.5);

	bufferSize = buffer_width * buffer_height * 3;
	buffer = new unsigned char[ bufferSize ];
	
	// separate objects into bounded and unbounded
	scene->initScene();
	
	// Add any specialized scene loading code here
	
	m_bSceneLoaded = true;

	return true;
}

void RayTracer::traceSetup( int w, int h )
{
	if( buffer_width != w || buffer_height != h )
	{
		buffer_width = w;
		buffer_height = h;

		bufferSize = buffer_width * buffer_height * 3;
		delete [] buffer;
		buffer = new unsigned char[ bufferSize ];
	}
	memset( buffer, 0, w*h*3 );
}

void RayTracer::traceLines( int start, int stop )
{
	vec3f col;
	if( !scene )
		return;

	if( stop > buffer_height )
		stop = buffer_height;

	for( int j = start; j < stop; ++j )
		for( int i = 0; i < buffer_width; ++i )
			tracePixel(i,j);
}
struct Point {
public:
	double x;
	double y;
};
vec3f RayTracer::traceCorners(double x, double y, double sideX, double sideY, int depth, double adaptThres) {
	// returns final colors between 0 and 1
	if (depth <= 0) {
		return trace(scene, x, y);
	}

	Point corners[4];
	corners[0] = { x - 0.5 * sideX, y - 0.5 * sideY };
	corners[1] = { x + 0.5 * sideX, y - 0.5 * sideY };
	corners[2] = { x - 0.5 * sideX, y + 0.5 * sideY };
	corners[3] = { x + 0.5 * sideX, y + 0.5 * sideY };

	vec3f centerCol = trace(scene, x, y);
	vec3f col = { 0,0,0 };

	for (int i = 0; i <= 3; i++) {
		vec3f cornerCol = trace(scene, corners[i].x, corners[i].y);
		if ((centerCol[0] - cornerCol[0] > adaptThres)
			|| (centerCol[1] - cornerCol[1] > adaptThres)
			|| (centerCol[2] - cornerCol[2] > adaptThres)
			) {
			col += 0.25 * traceCorners((x + corners[i].x) / 2, (y + corners[i].y) / 2, sideX / 2, sideY / 2, depth - 1, adaptThres);
		}
		else {

			col += 0.25 * cornerCol;

		}
	}

	return col;
}
void RayTracer::tracePixel( int i, int j )
{
	vec3f col;

	if( !scene )
		return;
	if (traceUI->superSamper() == true) {
		int size = traceUI->supSampleSize();
		vec3f sum = vec3f(0, 0, 0);
		for (int local_i = 0; local_i < size; local_i++) {
			for (int local_j = 0; local_j < size; local_j++) {
				double x = double(i + local_i * (1.0 / (double(size))) - 0.5) / double(buffer_width);
				double y = double(j + local_j * (1.0 / (double(size))) - 0.5) / double(buffer_height);
				sum += trace(scene, x, y);
			}
		}
		col = sum / (double(size * size));
	}
	else if (traceUI->adaptSup() == true) {
		double adaptThres = 0.1;
		int depth = 2;

		double x, y;
		x = double(i) / double(buffer_width);
		y = double(j) / double(buffer_height);
		col = traceCorners(x, y, 1 / double(buffer_width), 1 / double(buffer_height), depth, adaptThres);
		

		unsigned char* pixel = buffer + (i + j * buffer_width) * 3;

		pixel[0] = (int)(255.0 * col[0]);
		pixel[1] = (int)(255.0 * col[1]);
		pixel[2] = (int)(255.0 * col[2]);
	}
	else {
		double x = double(i) / double(buffer_width);
		double y = double(j) / double(buffer_height);

		col = trace(scene, x, y);
	}

	unsigned char *pixel = buffer + ( i + j * buffer_width ) * 3;

	pixel[0] = (int)( 255.0 * col[0]);
	pixel[1] = (int)( 255.0 * col[1]);
	pixel[2] = (int)( 255.0 * col[2]);
}


bool RayTracer::load_background(char* fn)
{
	// from the project one impressionist, load image
	unsigned char* data;
	int				width, height;

	if ((data = readBMP(fn, width, height)) == NULL)
	{
		fl_alert("Can't load background file");
		return false;
	}
	if (background != NULL)
		delete[] background;
	m_nBackWidth = width;
	m_nBackHeight = height;
	background = data;
	return true;
}

vec3f RayTracer::getBackgroundColor(double u, double v)
{
	if (u < 0 || u > 1 || v < 0 || v > 1) {
		return vec3f(1, 1, 1);
	}
	int x = u * m_nBackWidth;
	int y = v * m_nBackHeight;
	//printf("%d %d\n", x, y);
	double r = background[3 * (x + y * m_nBackWidth)] / 255.0;
	double g = background[3 * (x + y * m_nBackWidth) + 1] / 255.0;
	double b = background[3 * (x + y * m_nBackWidth) + 2] / 255.0;
	//printf("%lf %lf %lf\n", r, g, b);
	return vec3f(r, g, b);
}
