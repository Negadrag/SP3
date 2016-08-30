#ifndef PAUSECURSOR_H
#define PAUSECURSOR_H

#include "Cursor.h"
#include "GUI.h"

class PauseCursor : public Cursor
{
public:
	PauseCursor();
	~PauseCursor();

	void Init();
	void Update(OrthoCamera &camera, const double &dt);
	void Clear();
private:
	bool bLButtonState;
	void Clicking();
	void HotKeys();
	GUI* temp;
};

#endif