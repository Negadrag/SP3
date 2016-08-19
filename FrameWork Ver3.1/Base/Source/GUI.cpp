#include "GUI.h"

GUI::GUI(string s)
{
	position.Set(0, 0);
	buttonSize.Set(1,1);
	textOffset.Set(0, 0);
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