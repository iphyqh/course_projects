#pragma warning(disable : 4786)

#include "particleSystem.h"
#include "modelerview.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <limits.h>
#include <FL/gl.h>
#include <FL/glut.h>
#include <GL/glu.h>
#include <time.h> 

#define PI 3.14159265

/***************
 * Constructors
 ***************/

ParticleSystem::ParticleSystem(float g, float viscous)
{
	// TODO
	forceCoeffs.push_back(g); 
	forceCoeffs.push_back(viscous);
}


/*************
 * Destructor
 *************/

ParticleSystem::~ParticleSystem() 
{
	// TODO
	clearBaked();
}


/******************
 * Simulation fxns
 ******************/

/** Start the simulation */
void ParticleSystem::startSimulation(float t)
{
	// TODO
	clearBaked();
	bake_start_time = t; // Start at t
	// These values are used by the UI ...
	// -ve bake_end_time indicates that simulation
	// is still progressing, and allows the
	// indicator window above the time slider
	// to correctly show the "baked" region
	// in grey.
	bake_end_time = -1;
	simulate = true;
	dirty = true;

}

/** Stop the simulation */
void ParticleSystem::stopSimulation(float t)
{
	// TODO
	bake_end_time = t; // End at t
	// These values are used by the UI
	simulate = false;
	dirty = true;
	particles.clear(); // Should clear all particles when the simulation is ended, or when another round of simulation begins, the old particles appear again
}

/** Reset the simulation */
void ParticleSystem::resetSimulation(float t)
{
	// TODO
	clearBaked();
	// These values are used by the UI
	simulate = false;
	dirty = true;

}

/** Compute forces and update particles **/ // Should be done
void ParticleSystem::computeForcesAndUpdateParticles(float t)
{
	// TODO
	bake_fps = t - time; // Current frame time - last frame time, though it is not physical delta time, and it is frame dependent
	time = t; // Time increment
	if (isSimulate() && !isBaked(t)) {
		// Upadte particle states
		std::vector<Particle>::iterator DeadParticle = particles.begin();
		for (std::vector<Particle>::iterator particle = particles.begin(); particle < particles.end(); particle++) {
			particle->setForce(particle->getForce() + Vec3f(0, -forceCoeffs[0] * particle->getMass(), 0));
			particle->setForce(particle->getForce() + -forceCoeffs[1] * particle->getVelocity());

			particle->setVelocity(particle->getVelocity() + bake_fps * particle->getForce() / particle->getMass());
			particle->setPosition(particle->getPosition() + bake_fps * particle->getVelocity());

			particle->setForce(Vec3f(0, 0, 0));
			particle->setLife(particle->getLife() - bake_fps);	
		}
		for (int i = 0; i < particles.size(); i++)
		{
			if (!particles[i].getState())
				particles.erase(particles.begin() + i);
		}

		bakeParticles(t);
	}
}


void ParticleSystem::SpawnParticles(Mat4f WorldMatrix, int Number) {
	Vec4f WorldPosition = WorldMatrix * Vec4f(0, 0, 0, 1);
	if (isSimulate()) {
			for (int i = 0; i < Number; i++) {
				Particle p(Vec3f(WorldPosition[0], WorldPosition[1], WorldPosition[2]), Vec3f(5*(cos(time+ (rand() % 20) * PI / 180)), 0, 5*(sin(time + (rand() % 20) * PI / 180))), 1, 2);
				particles.push_back(p);
			}
	}
}

/** Render particles */
void ParticleSystem::drawParticles(float t)
{
	// TODO
	if (isSimulate()) {
		for (std::vector<Particle>::iterator particle = particles.begin(); particle < particles.end(); particle++) {
			particle->draw();
		}
	}
	else if (t >= bake_start_time && t <= bake_end_time)
	{
		map<float, std::vector<Particle>>::iterator particlesT = bakedParticles.lower_bound(t);
		if (particlesT != bakedParticles.end())
		for (std::vector<Particle>::iterator particle = particlesT->second.begin(); particle != particlesT->second.end(); particle++) {
			particle->draw();
		}
	}
	
	
}


/** Adds the current configuration of particles to
  * your data structure for storing baked particles **/
void ParticleSystem::bakeParticles(float t)
{
	// TODO
	bakedParticles.insert({ t, particles });
}

/** Clears out your data structure of baked particles */
void ParticleSystem::clearBaked()
{
	cout << "cleared" << endl;
	// TODO
	bakedParticles.clear();
}






