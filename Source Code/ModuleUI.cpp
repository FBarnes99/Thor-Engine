#include "Application.h"
#include "ModuleUI.h"
#include "ModuleWindow.h"

#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_sdl_gl3.h"

ModuleUI::ModuleUI(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleUI::~ModuleUI()
{}

bool ModuleUI::Init()
{
	ImGui_ImplSdlGL3_Init(App->window->window);

	ImGuiIO& io = ImGui::GetIO();
	io.IniFilename = "imgui.ini";

	test_color = ImColor(114, 144, 154);
	return true;
}

update_status ModuleUI::Update(float dt)
{
	ImGui_ImplSdlGL3_NewFrame(App->window->window);
	ImGui::ShowTestWindow();
	ImGuiIO& io = ImGui::GetIO();

	ImGui::Text("Hello, world!");


	if (ImGui::BeginMainMenuBar())
	{
		ImGui::MenuItem("File");
		ImGui::MenuItem("View");
		ImGui::MenuItem("Help");
		ImGui::EndMainMenuBar();
	}

	if (ImGui::Button("Quit", ImVec2(100, 50)))
	{
		return UPDATE_STOP;
	}

	char* button_label = "Open Window";
	if (show_second_window)
	{
		button_label = "Close Window";
	}

	if (ImGui::Button(button_label, ImVec2(100, 50)))
	{
		show_second_window ^= 1;
	}

	if (show_second_window)
	{
		//Setting up window flags
		ImGuiWindowFlags window_flags = 0;
		//Allowing menu bar on the window
		window_flags |= ImGuiWindowFlags_MenuBar;

		//ImGui::SetNextWindowSize(ImVec2(560, 680), ImGuiSetCond_FirstUseEver);
		ImGui::Begin("this is a damn window", &show_second_window, ImVec2(500, 300), 1.0f, window_flags);
		ImGui::Text("Hi there niggz!");

		//Menu Bar
		if (ImGui::BeginMenuBar())
		{
			//Menu Bar -- Menu Button
			if (ImGui::BeginMenu("Menu"))
			{
				ImGui::MenuItem("Disabled button", NULL, false, false);
				ImGui::MenuItem("Menu option 1");
				ImGui::MenuItem("Menu option 2");
				ImGui::MenuItem("Menu option 3");
				ImGui::MenuItem("Menu option 4");
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Edit"))
			{
				ImGui::MenuItem("Nothing in here, sorry!", NULL, false, false);
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		
		ImGui::ColorEdit3("Change Bg color", (float*)&test_color);
		ImGui::End();
	}

	//Change background color, we use "test_color", controllable variable from UI
	glClearColor(test_color.x, test_color.y, test_color.z, test_color.w);

	ImGui::Render();
	return UPDATE_CONTINUE;
}

bool ModuleUI::CleanUp()
{
	ImGui_ImplSdlGL3_Shutdown();
	return true;
}
