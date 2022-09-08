#include <cmath>
#include <algorithm>
#include "light.h"

double DirectionalLight::distanceAttenuation( const vec3f& P ) const
{
	// distance to light is infinite, so f(di) goes to 0.  Return 1.
	return 1.0;
}


vec3f DirectionalLight::shadowAttenuation(const vec3f& P) const
{
	// YOUR CODE HERE:
	// You should implement shadow-handling code here.
	ray light(P /*+ RAY_EPSILON * getDirection(P)*/, getDirection(P));
	isect i;
	if (scene->intersect(light, i));
	{
		if (fabs(i.t) < RAY_EPSILON)
			return vec3f(1.0, 1.0, 1.0); // No shadow
		else if (i.getMaterial().kt.length() > 0)
			return prod(vec3f(1.0, 1.0, 1.0), i.getMaterial().kt);
		else
			return vec3f(0.0, 0.0, 0.0); // Opaque shadow
	}

	return vec3f(1.0, 1.0, 1.0);
}

vec3f DirectionalLight::getColor( const vec3f& P ) const
{
	// Color doesn't depend on P 
	return color;
}

vec3f DirectionalLight::getDirection( const vec3f& P ) const
{
	return -orientation;
}

double PointLight::distanceAttenuation( const vec3f& P ) const
{
	// YOUR CODE HERE

	// You'll need to modify this method to attenuate the intensity 
	// of the light based on the distance between the source and the 
	// point P.  For now, I assume no attenuation and just return 1.0
	double d = (position - P).length();
	double d2 = (position - P).length_squared();

	double ConstAtten = scene->ConstantAtten();
	double LinearAtten = scene->LinearAtten();
	double QuadricAtten = scene->QuadricAtten();

	// Only if the slider has not been changed, and the .ray file contains Attenuation coefficients, we set the coefficients to those from .ray file
	if ((ConstAtten == 1.00 && LinearAtten == 0.00 && QuadricAtten == 0.00) && (constantAttenuation != 0.0 || linearAttenuation != 0.0 || quadricAttenuation != 0.0)) {
		ConstAtten = constantAttenuation;
		LinearAtten = linearAttenuation;
		QuadricAtten = quadricAttenuation;
	}

	return std::min(1.0, 1.0 / (ConstAtten + d * LinearAtten + d2 * QuadricAtten));
	return 1.0;
}

vec3f PointLight::getColor( const vec3f& P ) const
{
	// Color doesn't depend on P 
	return color;
}

vec3f PointLight::getDirection( const vec3f& P ) const
{
	return (position - P).normalize();
}


vec3f PointLight::shadowAttenuation(const vec3f& P) const
{
	// YOUR CODE HERE:
	// You should implement shadow-handling code here.
	ray light(P /*+ RAY_EPSILON * getDirection(P)*/, getDirection(P));
	isect i;
	if (scene->intersect(light, i));
	{
		if (fabs(i.t) < RAY_EPSILON)
			return vec3f(1.0, 1.0, 1.0); // No shadow
		else if (i.getMaterial().kt.length() > 0)
			return prod(vec3f(1.0, 1.0, 1.0), i.getMaterial().kt);
		else
			return vec3f(0.0, 0.0, 0.0); // Opaque shadow
	} 
    
	return vec3f(1.0, 1.0, 1.0);
}


vec3f AmbientLight::getColor(const vec3f& P) const
{
	// Color doesn't depend on P 
	return color;
}

vec3f AmbientLight::getDirection(const vec3f& P) const
{
	return vec3f(0.0, 0.0, 0.0);
}

vec3f AmbientLight::shadowAttenuation(const vec3f& P) const
{
	// No shadowAttenuation for AmbientLight
	return vec3f(1, 1, 1);
}

double AmbientLight::distanceAttenuation(const vec3f& P) const
{
	// No distance Attenuation for ambient light neither
	return 1.0;
}
