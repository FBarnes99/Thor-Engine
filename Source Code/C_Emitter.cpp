/*#include "C_Emitter.h"
#include "Application.h"
#include "GameObject.h"
#include "M_ParticlesManager.h"
#include "M_Particles.h"
#include "PerfTimer.h"

C_Emitter::C_Emitter(GameObject* parent) : Component(Type::Emitter, gameObject, true)
{
	parent->AddComponent(this);
	App.
		->particle_manager->emitters.push_back(this);
}

C_Emitter::~C_Emitter()
{
}


void C_Emitter::Update()
{
	float time = timer.Read();

	if (ratio > 0.0f)
	{
		if (time >= ratio)
		{
			if (App->module_time->gameState == GameState::PLAYING)
			{
				int pos = App->particle_manager->GetLastParticle();
				App->particle_manager->particles[pos].SetActive(position, speed, rotation, size, life, &texture, color);
				particlesList.push_back(&App->particle_manager->particles[pos]);
			}
		}
	}
}*/

