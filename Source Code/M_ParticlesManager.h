/*#ifndef __MODULE_PARTICLES_MANAGER_H__
#define __MODULE_PARTICLES_MANAGER_H__

#include "Module.h"
#include "Color.h"
#include <list>
#include "M_Particles.h"

class C_Emitter;
class M_Particles;

#define MAX_PARTICLES 5000

class M_ParticlesManager : public Module
{
public:
	M_ParticlesManager( bool start_enabled = true);
	~M_ParticlesManager();

	int GetLastParticle();

public:
	
	std::list<C_Emitter*> emitters;
	M_Particles particles[MAX_PARTICLES];
	int lastUsedParticle = 0;
};

#endif // __MODULE_PARTICLES_MANAGER_H__*/
