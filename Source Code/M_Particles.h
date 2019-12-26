/*#ifndef __MODULE_PARTICLES_H__
#define __MODULE_PARTICLES_H__

#pragma once
#include "MathGeoLib\src\MathGeoLib.h"
#include "Module.h"
#include "Color.h"
#include <list>

class C_Emitter;
struct aiMaterial;
class R_Texture;
class R_Material;

class M_Particles : public Module
{
public:
	M_Particles(bool start_enabled = true);
	~M_Particles();

	void SetActive(float3 position, float3 speed, float2 rotation, float size, float life, R_Texture** texture, float4 color);

public:
	bool active = false;

	float life = 0.0f;
	float3 position = float3::zero;
	float3 speed = float3::zero;
	float2 rotation = float2::zero;
	float size = 0.0f;
	R_Texture** texture = nullptr;
	float4 color = float4::one;
};
#endif*/
