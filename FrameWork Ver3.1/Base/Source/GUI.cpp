#include "GUI.h"

GUI::GUI(string s)
{
	position.SetZero();
	buttonSize.Set(1,1);
	textOffset.SetZero();
	b_isActive = true;
	b_textActive = true;
	meshID = GEO_NULL;
	rotation.SetZero();
	scale.Set(1, 1, 1);
	Text = s;
	f_textSize = 3.f;
	b_buttonActive = true;
	m_sceneID = 0;
	functionID = 0;
	GUIManager::GetInstance()->AddGUI(this);
	parent = nullptr;
	textColor.Set(1, 1, 1);
	b_lightEnabled = true;
}

GUI::~GUI()
{
	GUIManager::GetInstance()->RemoveGUI(this);
}

void GUI::SetText(string s)
{
	this->Text = s;
}

string GUI::GetText()
{
	return Text;
}

void GUI::SetTextSize(float size)
{
	f_textSize = size;
}

float GUI::GetTextSize()
{
	return f_textSize;
}

void GUI::SetParent(GUI* parent)
{
	this->parent = parent;
}

GUI* GUI::GetParent()
{
	return parent;
}