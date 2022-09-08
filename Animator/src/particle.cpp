 // SAMPLE_SOLUTION
#include "particle.h"
#include "modelerview.h"
#include "modelerapp.h"
#include "modelerdraw.h"

Particle::Particle(Vec3f position, Vec3f velocity, float mass, float life):
position(position), velocity(velocity), accumulatedForce(Vec3f(0, 0, 0)), mass(mass), life(life)
{};

Particle::Particle() :
	position(Vec3f(0, 0, 0)), velocity(Vec3f(0, 0, 0)), accumulatedForce(Vec3f(0,0,0)), mass(0), life(30)
{};

void Particle::draw()
{
	glPushMatrix();
	glTranslated(position[0],position[1],position[2]);
	setDiffuseColor(1, 1, 1);
	drawBox(0.1, 0.1, 0.1);
	glPopMatrix();
}