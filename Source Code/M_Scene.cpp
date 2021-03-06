#include "Globals.h"
#include "Application.h"
#include "M_Scene.h"
#include "M_Camera3D.h"
#include "M_Input.h"
#include "M_Import.h"
#include "M_Editor.h"
#include "M_Renderer3D.h"
#include "M_FileSystem.h"

#include "GameObject.h"
#include "C_Camera.h"
#include "C_Transform.h"
#include "R_Mesh.h"

#include <gl/GL.h>
#include <gl/GLU.h>
//#include <stdio.h>
#include "C_Camera.h"
#include "Intersections.h"
#include "Config.h"
#include "M_Meshes.h"
#include "M_Materials.h"
#include "M_Resources.h"

#include "Quadtree.h"

#include <windows.h>
#include <shobjidl.h> 

#include "Time.h"

//#include <GLFW/glfw3.h>

M_Scene::M_Scene(bool start_enabled) : Module("Scene", start_enabled)
{
	root = new GameObject(nullptr, "root");
	root->uid = 0;
}

M_Scene::~M_Scene()
{
}

bool M_Scene::Init(Config& config)
{
	std::string newScene = config.GetString("Current Scene");

	if (newScene == "" || !App->fileSystem->Exists(newScene.c_str()))
	{
		CreateDefaultScene();
	}
	else
	{
		current_scene = newScene;
		App->LoadScene(current_scene.c_str());
	}

	quadtree = new Quadtree(AABB(vec(-80, -30, -80), vec(80, 30, 80)));

	return true;
}

// Load assets
bool M_Scene::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	cullingTimer_library = App->moduleEditor->AddTimer("Math library culling", "Culling");
	cullingTimer_normal = App->moduleEditor->AddTimer("Normal culling", "Culling");
	cullingTimer_optimized = App->moduleEditor->AddTimer("Optimized culling", "Culling");

	return ret;
}

// Load assets
bool M_Scene::CleanUp()
{
	LOG("Unloading scene");

	RELEASE(root);

	return true;
}

// Update
update_status M_Scene::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
	{
		CreateCamera();
	}

	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
	{
		App->moduleResources->LoadPrefab("Assets/Models/Street_environment_V01.FBX");
	}

	if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN)
	{
		//Imports asset in project and loads it in scene
		std::string file = "Assets/Models/Street_environment_V01.FBX";
		App->moduleResources->ImportScene(file.c_str());
		//App->moduleResources->LoadPrefab(file.c_str());
	}

#pragma region WindowTest
	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN)
	{

		HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
			COINIT_DISABLE_OLE1DDE);
		if (SUCCEEDED(hr))
		{
			IFileOpenDialog *pFileOpen;
			// Create the FileOpenDialog object.
			hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
				IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

			if (SUCCEEDED(hr))
			{
				// Show the Open dialog box.
				hr = pFileOpen->Show(NULL);
			}
		}
	}
#pragma endregion

	glColor4f(1.0, 1.0, 1.0, 1.0);
	if (drawGrid)
	{
		//TODO: Move this into a mesh "prefab" or a renderer method
		glLineWidth(1.0f);

		glBegin(GL_LINES);

		float d = 20.0f;

		for (float i = -d; i <= d; i += 1.0f)
		{
			glVertex3f(i, 0.0f, -d);
			glVertex3f(i, 0.0f, d);
			glVertex3f(-d, 0.0f, i);
			glVertex3f(d, 0.0f, i);
		}

		glEnd();
	}

	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN)
	{
		drawGrid = !drawGrid;
	}

	UpdateAllGameObjects(root);

	if (App->renderer3D->culling_camera)
	{
		std::vector<const GameObject*> candidates;

		App->moduleEditor->StartTimer(cullingTimer_optimized);
		quadtree->CollectCandidates(candidates, App->renderer3D->culling_camera->frustum);

		std::vector<const GameObject*> gameObjects;
		TestGameObjectsCulling(candidates, gameObjects);
		TestGameObjectsCulling(nonStatic, gameObjects);
		App->moduleEditor->ReadTimer(cullingTimer_optimized);

		for (uint i = 0; i < gameObjects.size(); i++)
		{
			if (gameObjects[i]->name != "root");
			gameObjects[i]->Draw(App->moduleEditor->shaded, App->moduleEditor->wireframe, drawBounds, drawBoundsSelected);
		}
		gameObjects.clear();
	}
	else
	{
		DrawAllGameObjects(root);
	}

	if (drawQuadtree)
		quadtree->Draw();

	return UPDATE_CONTINUE;
}

update_status M_Scene::PostUpdate(float dt)
{
	DeleteToRemoveGameObjects();
	return UPDATE_CONTINUE;
}

GameObject* M_Scene::GetRoot()
{
	return root;
}

const GameObject* M_Scene::GetRoot() const
{
	return root;
}

void M_Scene::SaveConfig(Config& config) const
{
	config.SetString("Current Scene", current_scene.c_str());
}

void M_Scene::SetStaticGameObject(GameObject* gameObject, bool isStatic, bool allChilds)
{
	if (gameObject->isStatic != isStatic)
	{
		gameObject->SetStatic(isStatic);

		if (isStatic == true)
		{
			GameObject* it = gameObject->parent;
			while (it != nullptr && it->name != "root")
			{
				SetStaticGameObject(it, isStatic, false);
				it = it->parent;
			}

			quadtree->AddGameObject(gameObject);

			for (std::vector<const GameObject*>::iterator it = nonStatic.begin(); it != nonStatic.end(); it++)
			{
				if ((*it) == gameObject)
				{
					nonStatic.erase(it);
					break;
				}
			}
		}
		else
		{
			quadtree->RemoveGameObject(gameObject);
			nonStatic.push_back(gameObject);
		}
	}

	if (allChilds)
	{
		for (uint i = 0; i < gameObject->childs.size(); i++)
		{
			SetStaticGameObject(gameObject->childs[i], isStatic, allChilds);
		}
	}
}

void M_Scene::LoadConfig(Config& config)
{
	std::string newScene = config.GetString("Current Scene");

	if (newScene != "" && newScene != current_scene)
	{
		current_scene = newScene;
		App->LoadScene(current_scene.c_str());
	}
}

void M_Scene::SaveScene(Config& node) const
{
	//Store all gameObjects in a vector
	std::vector<GameObject*> gameObjects;
	GettAllGameObjects(gameObjects, root);

	App->moduleImport->SaveGameObjectConfig(node, gameObjects);
}

void M_Scene::LoadScene(Config& node, bool tmp)
{
	DeleteAllGameObjects();
	quadtree->Clear();
	std::vector<GameObject*> roots;
	std::vector<GameObject*> newGameObjects;

	App->moduleImport->LoadGameObjectConfig(node, roots);

	for (uint i = 0; i < roots.size(); i++)
	{
		roots[i]->parent = root;
		root->childs.push_back(roots[i]);
		GettAllGameObjects(newGameObjects, roots[i]);
	}

	for (uint i = 0; i < newGameObjects.size(); i++)
	{
		if (newGameObjects[i]->isStatic)
		{
			quadtree->AddGameObject(newGameObjects[i]);
		}
		else
		{
			nonStatic.push_back(newGameObjects[i]);
		}
	}
}

void M_Scene::LoadScene(const char* file)
{
	current_scene = file;
	App->LoadScene(file);
}

void M_Scene::LoadGameObject(uint64 ID)
{
	GameObject* gameObject = App->moduleImport->LoadGameObject(ID);
	if (gameObject != nullptr)
	{
		gameObject->parent = root;
		root->childs.push_back(gameObject);

		std::vector<GameObject*> newGameObjects;
		GettAllGameObjects(newGameObjects, gameObject);

		for (uint i = 0; i < newGameObjects.size(); i++)
		{
			if (newGameObjects[i]->isStatic)
			{
				quadtree->AddGameObject(newGameObjects[i]);
			}
			else
			{
				nonStatic.push_back(newGameObjects[i]);
			}
		}
	}
	else
	{
		LOG("[error] File '%s' could not be loaded");
	}
}

GameObject* M_Scene::CreateGameObject(const char* name)
{
	return new GameObject(root, name);
}

void M_Scene::DeleteGameObject(GameObject* gameObject)
{
	bool add = true;
	for (uint i = 0; i < toRemove.size(); i++)
	{
		if (toRemove[i] == gameObject)
			return;
	}
	toRemove.push_back(gameObject);
	App->OnRemoveGameObject(gameObject);

	while (gameObject->childs.empty() == false) //"DeleteGameObject" will remove the gameObject from the childs list
	{
		DeleteGameObject(gameObject->childs[0]);
	}
}

void M_Scene::OnRemoveGameObject(GameObject* gameObject)
{
	//Remove from quadtree // non-static vector
	if (quadtree->RemoveGameObject(gameObject) == false)
	{
		bool erased = false;
		for (std::vector<const GameObject*>::iterator it = nonStatic.begin(); it != nonStatic.end(); it++)
		{
			if (*it == gameObject)
			{
				nonStatic.erase(it);
				erased = true;
				break;
			}
		}
		if (erased == false)
			LOG("[warning] deleted GameObject not found in quadtree nor non-static vector");
	}

	//Removing parent child
	if (gameObject->parent)
	{
		GameObject* parent = gameObject->parent;
		for (std::vector<GameObject*>::iterator it = parent->childs.begin(); it != parent->childs.end();)
		{
			*it == gameObject ? it = parent->childs.erase(it) : it++;
		}
	}
}

void M_Scene::OnClickSelection(const LineSegment& segment)
{
	//Collecting quadtree GameObjects
	std::map<float, const GameObject*> candidates;
	quadtree->CollectCandidates(candidates, segment);

	//Collecting non-static GameObjects
	for (uint i = 0; i < nonStatic.size(); i++)
	{
		if (segment.Intersects(nonStatic[i]->GetAABB()))
		{
			float hit_near, hit_far;
			if (segment.Intersects(nonStatic[i]->GetOBB(), hit_near, hit_far))
				candidates[hit_near] = nonStatic[i];
		}
	}


	const GameObject* toSelect = nullptr;
	for (std::map<float, const GameObject*>::iterator it = candidates.begin(); it != candidates.end() && toSelect == nullptr; it++)
	{
		//Testing triangle by triangle
		const C_Mesh* mesh = it->second->GetComponent<C_Mesh>();
		if (mesh)
		{
			const R_Mesh* rMesh = (R_Mesh*)mesh->GetResource();

			if (rMesh)
			{
				LineSegment local = segment;
				local.Transform(it->second->GetComponent<C_Transform>()->GetGlobalTransform().Inverted());
				for (uint v = 0; v < rMesh->buffersSize[R_Mesh::b_indices]; v += 3)
				{
					uint indexA = rMesh->indices[v] * 3;
					vec a(&rMesh->vertices[indexA]);

					uint indexB = rMesh->indices[v + 1] * 3;
					vec b(&rMesh->vertices[indexB]);

					uint indexC = rMesh->indices[v + 2] * 3;
					vec c(&rMesh->vertices[indexC]);

					Triangle triangle(a, b, c);

					if (local.Intersects(triangle, nullptr, nullptr))
					{
						toSelect = it->second;
						break;
					}
				}
			}
		}
	}
	App->moduleEditor->SelectGameObject((GameObject*)toSelect);
}

void M_Scene::CreateCamera()
{
	GameObject* camera = new GameObject(root, "Camera");
	camera->GetComponent<C_Transform>()->SetPosition(float3(10, 10, 0));
	camera->CreateComponent(Component::Type::Camera);
	camera->GetComponent<C_Camera>()->Look(float3(0, 5, 0));
	camera->uid = random.Int();
}

void M_Scene::Play()
{
	Time::Start(60);
	App->SaveScene("ProjectSettings/tmp.scene", true);
}

void M_Scene::Stop()
{
	Time::Stop();
	App->LoadScene("ProjectSettings/tmp.scene");
}

void M_Scene::TestGameObjectsCulling(std::vector<const GameObject*>& vector, std::vector<const GameObject*>& final)
{
	for (uint i = 0; i < vector.size(); i++)
	{
		if (App->renderer3D->culling_camera->frustum.Intersects(vector[i]->GetAABB()))
		{
			final.push_back(vector[i]);
		}
	}
}

void M_Scene::UpdateAllGameObjects(GameObject* gameObject)
{
	root->Update();
}

void M_Scene::DrawAllGameObjects(GameObject* gameObject)
{
	if (gameObject->name != "root");
		gameObject->Draw(App->moduleEditor->shaded, App->moduleEditor->wireframe, drawBounds, drawBoundsSelected);

	for (uint i = 0; i < gameObject->childs.size(); i++)
	{
		DrawAllGameObjects(gameObject->childs[i]);
	}
}

void M_Scene::GettAllGameObjects(std::vector<GameObject*>& vector, GameObject* gameObject) const
{
	if (gameObject->name != "root")
		vector.push_back(gameObject);
	for (uint i = 0; i < gameObject->childs.size(); i++)
	{
		GettAllGameObjects(vector, gameObject->childs[i]);
	}
}

void M_Scene::FindGameObjectByID(uint id, GameObject* gameObject, GameObject** ret)
{
	if (gameObject->uid == id)
	{
		if (*ret != nullptr)
			LOG("[error] Conflict: GameObjects with same UID");
		*ret = gameObject;
	}

	//Optimization vs security: if ret != nullptr we can stop searching
	//If we search for all game objects, we can find uid conflicts if any
	for (uint i = 0; i < gameObject->childs.size(); i++)
	{
		FindGameObjectByID(id, gameObject->childs[i], ret);
	}

}

void M_Scene::DeleteAllGameObjects()
{
	for (uint i = 0; i < root->childs.size(); i++)
	{
		RELEASE(root->childs[i]);
	}
	root->childs.clear();
}

void M_Scene::CreateDefaultScene()
{
	App->LoadScene("ProjectSettings/Untitled.scene");
}

void M_Scene::DeleteToRemoveGameObjects()
{
	for (std::vector<GameObject*>::iterator it = toRemove.begin(); it != toRemove.end(); it++)
	{
		RELEASE(*it);
	}
	toRemove.clear();
}