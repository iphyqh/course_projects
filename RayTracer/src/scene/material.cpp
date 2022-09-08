#include "ray.h"
#include "material.h"
#include "light.h"

// Apply the phong model to this point on the surface of the object, returning
// the color of that point.
vec3f Material::shade( Scene *scene, const ray& r, const isect& i ) const
{
	// YOUR CODE HERE

	// For now, this method just returns the diffuse color of the object.
	// This gives a single matte color for every distinct surface in the
	// scene, and that's it.  Simple, but enough to get you started.
	// (It's also inconsistent with the phong model...)

	// Your mission is to fill in this method with the rest of the phong
	// shading model, including the contributions of all the light sources.
    // You will need to call both distanceAttenuation() and shadowAttenuation()
    // somewhere in your code in order to compute shadows and light falloff.

	vec3f Color(0.0, 0.0, 0.0);
	vec3f V = -(r.getDirection()).normalize();
	vec3f N = i.N.normalize();
	

	Color += ke;

	list<Light*>::const_iterator  liter;

	i.N;

	for (liter = scene->beginLights(); liter != scene->endLights(); liter++) {


		vec3f Intensity = (*liter)->getColor(r.at(i.t));
		vec3f L = (*liter)->getDirection(r.at(i.t)).normalize();
		vec3f R = (N.dot(L) * 2 * N - L).normalize();

		int ambient = 0;

		// Not an Ambient Light
		if (L.length() > 0.00001)
		{
			double distAtten = (*liter)->distanceAttenuation(r.at(i.t));
			vec3f shadowAtten = (*liter)->shadowAttenuation(r.at(i.t) + RAY_EPSILON * (*liter)->getDirection(r.at(i.t)));
			vec3f transparent(0.0, 0.0, 0.0);

			if (kt.length() > 0)
				transparent = kt;
			// Diffuse Reflection
			Color += prod(prod(prod(kd, Intensity) * std::max(0.0, N.dot(L)) * distAtten , shadowAtten), vec3f(1.0,1.0,1.0)-transparent);

			// Specular Reflection
			Color += prod(prod(prod(ks, Intensity) * (std::pow(std::max(0.0, R.dot(V)), shininess * 128)) * distAtten, shadowAtten), vec3f(1.0,1.0,1.0)-transparent);
		}
		else // There is an Ambient Light
		{
			Color += scene->AmbientLight() * prod(Intensity,ka); // Need a slider to control coefficient before kaIa
			ambient = 1;
		}
		
		if (ambient == 0) {
			// Constant Ambient
			Color += 0.5 * scene->AmbientLight() * ka; // 0.2 stands for ambient slider
		}

	}

	Color.clamp();

	return Color;
}
