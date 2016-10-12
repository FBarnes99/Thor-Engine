#include "ModuleMaterials.h"
#include "C_Material.h"

#include "Devil\include\ilu.h"
#include "Devil\include\ilut.h"

#pragma comment( lib, "Devil/libx86/DevIL.lib" )
#pragma comment( lib, "Devil/libx86/ILU.lib" )
#pragma comment( lib, "Devil/libx86/ILUT.lib" )

ModuleMaterials::ModuleMaterials(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleMaterials::~ModuleMaterials()
{

}

bool ModuleMaterials::Init()
{
	return true;
}

bool ModuleMaterials::CleanUp()
{
	return true;
}

/*Search for a material with same texture path
if a material is found, returns the material
otherwise returns NULL*/
C_Material* ModuleMaterials::Exists(const char* texture_path) const
{
	std::list<C_Material*>::const_iterator it = materials.begin();
	while (it != materials.end())
	{
		if ((*it)->texture_path == texture_path)
			return (*it);
		it++;
	}
	return NULL;
}

C_Material* ModuleMaterials::LoadMaterial(const char* texture_path, const char* file)
{
	C_Material* material = NULL;
	material = Exists(texture_path);
	if (material != NULL)
	{
		return material;
	}
	else
	{
		material = new C_Material(NULL);
		material->texture_path = texture_path;
		material->texture_file = file;
		material->texture_id = LoadIMG(material->texture_path.c_str());
		materials.push_back(material);
		materials_count++;
		LOG("Materials count: %i", materials_count);
	}
	
	return material;
}

uint ModuleMaterials::LoadIMG(const char* path)
{
	uint ret;
	ret = ilutGLLoadImage((char*)path);
	return ret;
}