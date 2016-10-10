#ifndef __ModuleEditor_H__
#define __ModuleEditor_H__

#include "Module.h"
#include "ImGui\imgui.h"

class PanelConsole;
class PanelHierarchy;
class PanelInspector;

class ModuleEditor : public Module
{
public:

	ModuleEditor(Application* app, bool start_enabled = true);
	~ModuleEditor();

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	bool CleanUp();

	void Log(const char* input);
	void GetEvent(SDL_Event* event);
	void UpdateFPSData(int fps, int ms);

	void OnResize(int screen_width, int screen_height);
private:

	void DrawPanels();
	void ShowAboutWindow();
	void ShowSettingsWindow();

	void InitFPSData();

public:
	//Panels
	PanelHierarchy* panelHierarchy = NULL;
	PanelConsole* panelConsole = NULL;
	PanelInspector* panelInspector = NULL;

private:
	char tmp_appName[26] = "Not working yet";
	char tmp_orgName[26] = "Not working yet";

	//Active window controllers
	bool show_About_window = false;
	bool show_Demo_window = false;
	bool show_Settings_window = true;

	//FPS data saving
	float FPS_data [100];
	float ms_data[100];
};

#endif
