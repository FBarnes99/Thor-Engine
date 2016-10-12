#include "PanelConsole.h"
#include <stdlib.h>

PanelConsole::PanelConsole()
{
	position.x = 2;
	position.y = 286;
	ClearLog();
}

PanelConsole::~PanelConsole()
{
	ClearLog();
}

void PanelConsole::ClearLog()
{
	for (int i = 0; i < items.Size; i++)
		free(items[i]);
	items.clear();
	scrollToBottom = true;
}

void PanelConsole::AddLog(const char* input)
{
	items.push_back(strdup(input));
	scrollToBottom = true;
}

void PanelConsole::Draw()
{
	if (active)
	{
		ImGui::SetNextWindowPos(ImVec2(position.x, position.y));
		ImGui::SetNextWindowSize(ImVec2(size.x, size.y));

		if (!ImGui::Begin("Console", &active, ImVec2(size.x, size.y), 1.0f, 0))
		{
			ImGui::End();
			return;
		}


		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 6)); // Tighten spacing

		ImColor col = ImColor(0.6f, 0.6f, 1.0f, 1.0f);

		for (int i = 0; i < items.Size; i++)
		{
			const char* item = items[i];
			ImGui::PushStyleColor(0, col);
			ImGui::TextUnformatted(item);
			ImGui::PopStyleColor();

		}

		if (scrollToBottom)
			ImGui::SetScrollHere(1.0f);

		ImGui::PopStyleVar();

		scrollToBottom = false;

		ImGui::End();
	}
}

void PanelConsole::UpdatePosition(int screen_width, int screen_height)
{
	position.x = 0;
	position.y = (screen_height * (0.75));

	size.y = screen_height - position.y;
	size.x = screen_width * (0.80);
}