/*#include "M_Particles.h"

#include "Application.h"
#include "M_FileSystem.h"

#include "C_Material.h"
#include "Color.h"
#include "R_Texture.h"
#include "R_Material.h"
#include "M_Resources.h"


M_Particles::M_Particles(bool start_enabled) : Module("Particle", start_enabled)
{
}

M_Particles::~M_Particles()
{
}

void M_Particles::SetActive(float3 position, float3 speed, float2 rotation, float size, float life, R_Texture** texture, float4 color)
{
	active = true;

	this->position = position;
	this->speed = speed;
	this->rotation = rotation * DEGTORAD;
	this->size = size;
	this->life = life;
	this->texture = texture;
	this->color = color;
}*/