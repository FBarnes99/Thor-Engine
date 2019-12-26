/*#include "M_Meshes.h"
#include "Application.h"

#include "C_Mesh.h"
#include "R_Mesh.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

#include "M_FileSystem.h"
#include "M_ParticlesManager.h"

#include "C_Emitter.h"
#include "M_Particles.h"


M_ParticlesManager::M_ParticlesManager(bool start_enabled) : Module("Particles", start_enabled)
{
}


M_ParticlesManager::~M_ParticlesManager()
{
}

int M_ParticlesManager::GetLastParticle()
{
	for (int i = lastUsedParticle; i < MAX_PARTICLES; i++) {
		if (particles[i].life < 0) {
			lastUsedParticle = i;
			return i;
		}
	}

	for (int i = 0; i < lastUsedParticle; i++) {
		if (particles[i].life < 0) {
			lastUsedParticle = i;
			return i;
		}
	}

	return 0;
}*/