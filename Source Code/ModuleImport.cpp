#include "Globals.h"
#include "Application.h"
#include "ModuleImport.h"
#include "ModuleScene.h"
#include "ModuleScene.h"

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



ModuleImport::ModuleImport(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

// Destructor
ModuleImport::~ModuleImport()
{}

// Called before render is available
bool ModuleImport::Init()
{
	LOG("Loading Module Import");

	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);

	const aiScene* file = aiImportFile("Game/maya tmp test.fbx", aiProcessPreset_TargetRealtime_MaxQuality);
	GameObject* go = LoadFBX(file, file->mRootNode, NULL);
	App->scene->AddGameObject(go);
	return true;
}
update_status ModuleImport::Update(float dt)
{
	return UPDATE_CONTINUE;
}

GameObject* ModuleImport::LoadFBX(const aiScene* scene, const aiNode* node, GameObject* parent)
{
	//TODO: get all transforms
	aiVector3D		translation;
	aiVector3D		scaling;
	aiQuaternion	rotation;
	
	//Decomposing transform matrix into translation rotation and scale
	node->mTransformation.Decompose(scaling, rotation, translation);

	float3 pos(translation.x, translation.y, translation.z);
	float3 scale(scaling.x, scaling.y, scaling.z);
	Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);

	//TODO: skipp all dummy modules. Assimp loads fbx nodes to stack all transformations
	/*
	std::string name = node->mName.C_Str();	
	static const char* dummies[5] = {
		"$AssimpFbx$_PreRotation", "$AssimpFbx$_Rotation", "$AssimpFbx$_PostRotation",
		"$AssimpFbx$_Scaling", "$AssimpFbx$_Translation" };

	for (int i = 0; i < 5; ++i)
	{
		//All dummy modules have one children (next dummy module or last module containing the mesh)
		if (name.find(dummies[i]) != std::string::npos && node->mNumChildren == 1)
		{
			node = node->mChildren[0];

			node->mTransformation.Decompose(scaling, rotation, translation);
			// accumulate transform
			pos += float3(translation.x, translation.y, translation.z);
			scale = float3(scale.x * scaling.x, scale.y * scaling.y, scale.z * scaling.z);
			rot = rot * Quat(rotation.x, rotation.y, rotation.z, rotation.w);

			name = node->mName.C_Str();
			i = -1; // start over!
		}
	}
	*/
	GameObject* gameObject = new GameObject(parent, pos, scale, rot);

	// Loading node meshes
	for (uint i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* newMesh = scene->mMeshes[node->mMeshes[i]];

		GameObject* child = new GameObject(parent);
		parent->childs.push_back(child);

		Mesh* go_mesh = new Mesh();

		//Loading mesh vertices data
		go_mesh->num_vertices = newMesh->mNumVertices;
		go_mesh->vertices = new float[go_mesh->num_vertices * 3];
		memcpy(go_mesh->vertices, newMesh->mVertices, sizeof(float) * go_mesh->num_vertices * 3);
		LOG("New mesh with %d vertices", go_mesh->num_vertices);

		//Loading mesh faces data
		if (newMesh->HasFaces())
		{
			go_mesh->num_indices = newMesh->mNumFaces * 3;
			go_mesh->indices = new uint[go_mesh->num_indices];
			for (uint i = 0; i < newMesh->mNumFaces; i++)
			{
				if (newMesh->mFaces[i].mNumIndices != 3)
				{
					LOG("WARNING, geometry face with != 3 indices!");
				}
				else
				{
					//Copying each face, we skip 3 slots in indices because an aiFace is made of 3 uints
					memcpy(&go_mesh->indices[i * 3], newMesh->mFaces[i].mIndices, 3 * sizeof(uint));
				}
			}
		}
		//Loading mesh normals data
		if (newMesh->HasNormals())
		{
			go_mesh->num_normals = newMesh->mNumVertices;
			go_mesh->normals = new float[go_mesh->num_normals * 3];
			memcpy(go_mesh->normals, newMesh->mNormals, sizeof(float) * go_mesh->num_normals * 3);
		}

		//Loading mesh texture coordinates
		if (newMesh->HasTextureCoords(0))
		{
			go_mesh->num_tex_coords = go_mesh->num_vertices;
			go_mesh->tex_coords = new float[newMesh->mNumVertices * 2];

			for (unsigned int i = 0; i < go_mesh->num_tex_coords; i++)
			{
				go_mesh->tex_coords[i * 2] = newMesh->mTextureCoords[0][i].x;
				go_mesh->tex_coords[i * 2 + 1] = newMesh->mTextureCoords[0][i].y;
			}
		}
		go_mesh->LoadBuffers();
		child->mesh = go_mesh;
	}

	for (uint i = 0; i < node->mNumChildren; i++)
	{
		LoadFBX(scene, node->mChildren[i], gameObject);
	}

	//GameObject* gameObject = new GameObject(*mesh);
	return gameObject;
}

uint ModuleImport::LoadIMG(char* path)
{
	uint ret;
	ret = ilutGLLoadImage(path);
	return ret;
}

// Called before quitting
bool ModuleImport::CleanUp()
{
	LOG("");

	return true;
}
