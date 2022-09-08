 // SAMPLE_SOLUTION

#ifndef __PARTICLE_H__
#define __PARTICLE_H__


#include "vec.h"
#include "camera.h"
class Particle {
private:
	Vec3f position;
	Vec3f velocity;
	Vec3f accumulatedForce; // Total force accumulated
	float mass;
	float life;

public:
	// Constructor
	// Default
	Particle();
	Particle(Vec3f position, Vec3f velocity, float mass, float life);
	
	// Accessors
	Vec3f getPosition() const { return position; }
	Vec3f getVelocity() const { return velocity; }
	Vec3f getForce() const { return accumulatedForce; }
	float getMass() const { return mass; }
	float getLife() const { return life; }
	bool getState() const { return life > 0; }
	// Mutators
	void setPosition(Vec3f newPosition) { position = newPosition; }
	void setVelocity(Vec3f newVelocity) { velocity = newVelocity; }
	void setForce(Vec3f newForce) { accumulatedForce = newForce; }
	void setLife(float newLife) { life = newLife; }
	// Util
	void draw();
};


#endif