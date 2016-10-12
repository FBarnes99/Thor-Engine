#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"

#include "ModuleInput.h"

//Panels
#include "PanelHierarchy.h"
#include "PanelConsole.h"
#include "PanelInspector.h"
#include "PanelConfiguration.h"

#include "OpenGL.h"

#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_sdl_gl3.h"

ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleEditor::~ModuleEditor()
{}

bool ModuleEditor::Init()
{
	ImGui_ImplSdlGL3_Init(App->window->window);

	ImGuiIO& io = ImGui::GetIO();
	io.IniFilename = NULL;
	io.MouseDrawCursor = true;


	int screen_width = GetSystemMetrics(SM_CXSCREEN);
	int screen_height = GetSystemMetrics(SM_CYSCREEN);

	//Initializing all panels
	panelConsole = new PanelConsole();
	panelHierarchy = new PanelHierarchy();
	panelInspector = new PanelInspector();
	panelConfiguration = new PanelConfiguration();
	//Change background color, we use "test_color", controllable variable from UI
	ImVec4 BgColor = ImColor(71, 71, 71);
	glClearColor(BgColor.x, BgColor.y, BgColor.z, BgColor.w);
	return true;
}

update_status ModuleEditor::PreUpdate(float dt)
{


	return UPDATE_CONTINUE;
}

update_status ModuleEditor::Update(float dt)
{
	ImGui_ImplSdlGL3_NewFrame(App->window->window);
	DrawPanels();

	//Showing all windows ----------
	if (show_About_window)
		ShowAboutWindow();
	if (show_Demo_window)
		ImGui::ShowTestWindow();
	if (show_Settings_window)
		ShowSettingsWindow();

	// -----------------------------
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit          ", "Esc"))
			{
				ImGui::EndMenu();
				return UPDATE_STOP;	
			}
			ImGui::EndMenu();

		}
		if (ImGui::BeginMenu("Window"))
		{
			if (ImGui::MenuItem("Console          ", "1", &panelConsole->active))
			{
			}
			if (ImGui::MenuItem("Settings         ", "2", &show_Settings_window))
			{
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			ImGui::MenuItem("About Thor Engine   ", NULL, &show_About_window);
			ImGui::Separator();
			if (ImGui::MenuItem("Documentation       "))
			{
				App->RequestBrowser("https://github.com/markitus18/Game-Engine/wiki");
			}
			if (ImGui::MenuItem("Download latest     "))
			{
				App->RequestBrowser("https://github.com/markitus18/Game-Engine/releases");
			}
			if (ImGui::MenuItem("Report a bug        "))
			{
				App->RequestBrowser("https://github.com/markitus18/Game-Engine/issues");
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Debugging Tools"))
		{
			ImGui::MenuItem("ImGui Demo", NULL, &show_Demo_window);
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
	//----------------------------

	ImGui::Render();
	return UPDATE_CONTINUE;
}

bool ModuleEditor::CleanUp()
{
	if (panelConsole)
	{
		delete panelConsole;
		panelConsole = NULL;
	}

	if (panelHierarchy)
	{
		delete panelHierarchy;
		panelHierarchy = NULL;
	}

	if (panelInspector)
	{
		delete panelInspector;
		panelInspector = NULL;
	}

	if (panelConfiguration)
	{
		delete panelConfiguration;
		panelConfiguration = NULL;
	}

	ImGui_ImplSdlGL3_Shutdown();
	return true;
}

void ModuleEditor::Log(const char* input)
{
	if (panelConsole != NULL)
		panelConsole->AddLog(input);
}

void ModuleEditor::GetEvent(SDL_Event* event)
{
	ImGui_ImplSdlGL3_ProcessEvent(event);
}

void ModuleEditor::DrawPanels()
{
	if (panelHierarchy != NULL)
	{
		panelHierarchy->Draw();
	}

	if (panelConsole != NULL)
	{
		panelConsole->Draw();
	}

	if (panelInspector != NULL)
	{
		panelInspector->Draw();
	}

	if (panelConfiguration != NULL)
	{
		panelConfiguration->Draw();
	}
}

void ModuleEditor::ShowAboutWindow()
{
	ImGui::Begin("About Thor Engine", &show_About_window, ImVec2(400, 100), 1.0f);
	ImGui::Text("v0.2-alpha");
	ImGui::Separator();
	ImGui::Text("By Marc Garrigo for educational purposes.");
	ImGui::Text("Thor Engine is licensed under Public Domain, see LICENSE for more information.");
	ImGui::End();
}

void ModuleEditor::ShowSettingsWindow()
{
	//ImGui::Begin("Settings", &show_Settings_window, ImVec2(500, 600), 1.0f);

}

void ModuleEditor::InitFPSData()
{
	for (int i = 0; i < 100; i++)
	{
		FPS_data[i] = 0;
		ms_data[i] = 0;
	}
}

void ModuleEditor::UpdateFPSData(int fps, int ms)
{
	for (int i = 0; i < 100; i++)
	{
		FPS_data[i] = FPS_data[i + 1];
		ms_data[i] = ms_data[i + 1];
	}
	FPS_data[100 - 1] = fps;
	ms_data[100 - 1] = ms;
}

void ModuleEditor::OnResize(int screen_width, int screen_height)
{
	panelConsole->UpdatePosition(screen_width, screen_height);
	panelHierarchy->UpdatePosition(screen_width, screen_height);
	panelInspector->UpdatePosition(screen_width, screen_height);
	panelHierarchy->UpdatePosition(screen_width, screen_height);
}