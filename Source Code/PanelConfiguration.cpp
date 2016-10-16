#include "PanelConfiguration.h"
#include "Application.h"
#include "ModuleInput.h"

PanelConfiguration::PanelConfiguration()
{
}

PanelConfiguration::~PanelConfiguration()
{

}

void PanelConfiguration::Draw(ImGuiWindowFlags flags)
{
	if (active)
	{
		ImGui::SetNextWindowPos(ImVec2(position.x, position.y));
		ImGui::SetNextWindowSize(ImVec2(size.x, size.y));

		if (!ImGui::Begin("Configuration", &active, ImVec2(size.x, size.y), 1.0f, flags))
		{
			ImGui::End();
			return;
		}
		if (ImGui::BeginMenu("Options"))
		{
			ImGui::MenuItem("Default", NULL, false, false);
			//if (ImGui::IsItemHovered())
			//	ImGui::SetMouseCursor(2);
			ImGui::MenuItem("Save", NULL, false, false);
			ImGui::MenuItem("Load", NULL, false, false);
			ImGui::EndMenu();
		}
		if (ImGui::CollapsingHeader("Application"))
		{
			//TODO: This will be moved into module editor and use listeners structure
			char appName[100];
			strcpy_s(appName, 100, App->GetWindowTitle());

			if (ImGui::InputText("Project Name", appName, 100, ImGuiInputTextFlags_EnterReturnsTrue))
				App->SetWindowTitle(appName);
			ImGui::PlotHistogram("FPS", FPS_data, IM_ARRAYSIZE(FPS_data), 0, NULL, 0.0f, 120.0f, ImVec2(0, 80));
			ImGui::PlotHistogram("MS", ms_data, IM_ARRAYSIZE(ms_data), 0, NULL, 0.0f, 40.0f, ImVec2(0, 80));
		}

		if (ImGui::CollapsingHeader("Window"))
		{

		}

		if (ImGui::CollapsingHeader("File System"))
		{

		}

		if (ImGui::CollapsingHeader("Input"))
		{
			ImGui::Text("Mouse position: %i, %i", App->input->GetMouseX(), App->input->GetMouseY());
			ImGui::Text("Mouse motion: %i, %i", App->input->GetMouseXMotion(), App->input->GetMouseYMotion());
			ImGui::Text("Mouse wheel: %i", App->input->GetMouseZ());
		}

		if (ImGui::CollapsingHeader("Hardware"))
		{

		}

		ImGui::End();
	}
}

void PanelConfiguration::UpdatePosition(int screen_width, int screen_height)
{
	position.x = screen_width * (0.80);
	position.y = screen_height * (0.60);

	size.x = screen_width - position.x;
	size.y = screen_height - position.y;
}

void PanelConfiguration::Init()
{
	for (int i = 0; i < 100; i++)
	{
		FPS_data[i] = 0;
		ms_data[i] = 0;
	}
}

void PanelConfiguration::UpdateFPSData(int fps, int ms)
{
	for (int i = 0; i < 100; i++)
	{
		FPS_data[i] = FPS_data[i + 1];
		ms_data[i] = ms_data[i + 1];
	}
	FPS_data[100 - 1] = fps;
	ms_data[100 - 1] = ms;
}

/*
ImGui::Begin("Settings", &show_Settings_window, ImVec2(500, 600), 1.0f);
if (ImGui::BeginMenu("Options"))
{
ImGui::MenuItem("Default", NULL, false, false);
if (ImGui::IsItemHovered())
ImGui::SetMouseCursor(2);
ImGui::MenuItem("Save", NULL, false, false);
ImGui::MenuItem("Load", NULL, false, false);
ImGui::EndMenu();
}

if (ImGui::CollapsingHeader("Application"))
{
ImGui::InputText("Project Name", tmp_appName, IM_ARRAYSIZE(tmp_appName));
ImGui::PlotHistogram("FPS", FPS_data, IM_ARRAYSIZE(FPS_data), 0, NULL, 0.0f, 120.0f, ImVec2(0, 80));
ImGui::PlotHistogram("MS", ms_data, IM_ARRAYSIZE(ms_data), 0, NULL, 0.0f, 40.0f, ImVec2(0, 80));
}

if (ImGui::CollapsingHeader("Window"))
{

}

if (ImGui::CollapsingHeader("File System"))
{

}

if (ImGui::CollapsingHeader("Input"))
{
ImGui::Text("Mouse position: %i, %i", App->input->GetMouseX(), App->input->GetMouseY());
ImGui::Text("Mouse motion: %i, %i", App->input->GetMouseXMotion(), App->input->GetMouseYMotion());
ImGui::Text("Mouse wheel: %i", App->input->GetMouseZ());

}

if (ImGui::CollapsingHeader("Hardware"))
{

}

ImGui::End();
*/