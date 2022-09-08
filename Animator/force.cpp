 // SAMPLE_SOLUTION
#include "particle.h"
Particle::Particle(Vec3f position, Vec3f velocity, Vec3f force, float mass):
position(position), velocity(velocity), force(force), mass(mass)
{};

Particle::Particle() :
	position(Vec3f(0, 0, 0)), velocity(Vec3f(0, 0, 0)), force(Vec3f(0,0,0)), mass(0)
{};