#ifndef CURSORCONTROL_H
#define CURSORCONTROL_H

class CursorControl
{
public:
	CursorControl();
	~CursorControl();
	void Update(double x, double y);

	double x, y;
};

#endif