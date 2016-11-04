#include "ModuleMaterials.h"

#include "Application.h"
#include "ModuleFileSystem.h"

#include "C_Material.h"
#include "Color.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

#include "Devil\include\ilu.h"
#include "Devil\include\ilut.h"

#pragma comment( lib, "Devil/libx86/DevIL.lib" )
#pragma comment( lib, "Devil/libx86/ILU.lib" )
#pragma comment( lib, "Devil/libx86/ILUT.lib" )



ModuleMaterials::ModuleMaterials(Application* app, bool start_enabled) : Module("Materials", start_enabled)
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
otherwise returns nullptr*/
C_Material* ModuleMaterials::Exists(const char* texture_path) const
{
	std::list<C_Material*>::const_iterator it = materials.begin();
	while (it != materials.end())
	{
		if ((*it)->texture_path == texture_path)
			return (*it);
		it++;
	}
	return nullptr;
}

C_Material* ModuleMaterials::LoadMaterial(const aiMaterial* from, const std::string& path)
{
	uint numTextures = from->GetTextureCount(aiTextureType_DIFFUSE);
	std::string mat_path = "";
	std::string file = "";
	//TODO: too much code just for the path, create a new function
	if (numTextures > 0)
	{
		aiString aiStr;
		aiReturn ret = from->GetTexture(aiTextureType_DIFFUSE, 0, &aiStr);
		App->fileSystem->SplitFilePath(aiStr.C_Str(), &mat_path, &file);
	}
	aiColor4D color;
	from->Get(AI_MATKEY_COLOR_DIFFUSE, color);

	C_Material* material = nullptr;
	//TODO START: if we avoid duplicating materials this causes the game to break
	//if (texture_path != "" && file != "")
	//{
	//	material = Exists(texture_path.c_str());
	//}
	//TODO END
	if (material != nullptr)
	{
		return material;
	}
	else
	{
		material = new C_Material(nullptr);
		if (mat_path != "" && file != "")
		{
			material->texture_path = mat_path + file;
			material->texture_file = file;
			material->texture_id = LoadIMG((char*)file.c_str());
		}
		else
		{
			material->texture_id = 0;
		}

		material->color = Color(color.r, color.g, color.b, color.a);
		materials.push_back(material);
	}
	
	return material;
}

uint ModuleMaterials::LoadIMG(char* file)
{
	//TODO: Search for all "Textures" folder and search that file
	uint ret = 0;

	char* buffer = nullptr;
	std::string full_path = "/Textures/";
	full_path.append(file);

	uint size = App->fileSystem->Load(full_path.c_str(), &buffer);

	ILuint ImageName;
	ilGenImages(1, &ImageName);
	ilBindImage(ImageName);

	if (ilLoadL(IL_TYPE_UNKNOWN, (const void*)buffer, size))
	{
		ret = ilutGLBindTexImage();
		ilDeleteImages(1, &ImageName);
	}

	RELEASE_ARRAY(buffer);
	return ret;
}

//Tmp function, move to file system
void ModuleMaterials::CutPath(std::string& str)
{
	uint position = str.find_last_of("\\/");
	if (position != std::string::npos)
	{
		str = str.substr(position + 1, str.size() - position);
	}
}

std::string ModuleMaterials::GetFileFolder(const std::string& str)
{
	std::string ret;
	uint position = str.find_last_of("\\/");
	if (position != std::string::npos)
	{
		ret = str.substr(0, position + 1);
	}
	return ret;
}