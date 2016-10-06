#ifndef __PANEL_HIERARCHY_H__
#define __PANEL_HIERARCHY_H__

#include "Panel.h"
#include <vector>

class GameObject;
typedef int ImGuiTreeNodeFlags;

class PanelHierarchy : public Panel
{
public:
	PanelHierarchy();
	~PanelHierarchy();

	void Draw();
	void DrawGameObject(GameObject* gameObject, ImGuiTreeNodeFlags default_flags);

	//GameObject selection methods
	void SelectSingle(GameObject* gameObject);
	void AddSelect(GameObject* gameObject);
	void UnselectSingle(GameObject* gameObject);
	void UnselectAll();
	
	std::vector<GameObject*> selectedGameObjects;
};

#endif