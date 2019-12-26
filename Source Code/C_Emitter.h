/*#include "Component.h"
#include "Timer.h"

#include "Globals.h"
#include "MathGeoLib\src\MathGeoLib.h"


struct aiMaterial;
class R_Texture;
class R_Material;
class M_Particles;

#include <list>
class C_Emitter : public Component
{
public:
	C_Emitter(GameObject* parent);
	~C_Emitter();

	void Update();

public:
	Timer timer;

	float ratio = 0.f;

	float life = 0.0f;
	float3 position = float3::zero;
	float3 speed = float3::zero;
	float2 rotation = float2::zero;
	float size = 0.0f;
	R_Texture* texture = nullptr;
	float4 color = float4::one;

	std::list<M_Particles*> particle_list;
};*/