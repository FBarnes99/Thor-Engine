#ifndef __TREE_DISPLAY_H__
#define __TREE_DISPLAY_H__

class TreeNode;

class TreeDisplay
{
public:
	TreeDisplay();
	~TreeDisplay();

private:
	void DrawTree(TreeNode* root);
	void DrawNode(TreeNode* node);
	void ShowNode(TreeNode* node);
	void DrawNodeChilds(TreeNode* node);

	void HandleUserInput(TreeNode* node);
	void HandleArrows();
	void DoShiftSelection(TreeNode* selected, bool select);

	bool IsHighlighted(TreeNode* node) const;
	bool SetParentByPlace(TreeNode* parent, std::vector<TreeNode*> childs, TreeNode* next = nullptr);

private:
	TreeNode* lastSelected = nullptr;
	bool selecting = false;
};

#endif