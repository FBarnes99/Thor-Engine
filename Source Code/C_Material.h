#ifndef __C_MATERIAL_H__
#define __C_MATERIAL_H__

#include "Component.h"
#include <string>
#include "Color.h"

class GameObject;

class C_Material : public Component
{
public:
	C_Material(const GameObject* gameObject);
	~C_Material();

	static Component::Type GetType();
};

#endif // __C_MATERIAL_H__
