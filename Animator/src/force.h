 // SAMPLE_SOLUTION

#ifndef __FORCE_H__
#define __FORCE_H__


#include "vec.h"

class Force {
private:
	Vec3f position;
	Vec3f velocity;
	Vec3f force; // Total force accumulated
	float mass;

public:
	// Constructor
	// Default
	Particle();
	Particle(Vec3f position, Vec3f velocity, Vec3f force, float mass);
	
	// Accessors
	Vec3f getPosition() const { return position; }
	Vec3f getVelocity() const { return velocity; }
	Vec3f getForce() const { return force; }
	float getMass() const { return mass; }

	// Mutators
	void setPosition(Vec3f newPosition) { position = newPosition; }
	void setVelocity(Vec3f newVelocity) { velocity = newVelocity; }
	void setForce(Vec3f newForce) { force = newForce; }
};


#endif