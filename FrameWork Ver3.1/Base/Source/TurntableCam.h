#ifndef TURNTABLE_CAM_H
#define TURNTABLE_CAM_H

#include "Camera3.h"


class TurntableCam : public Camera3
{
public:

	TurntableCam();
	~TurntableCam();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt);
	virtual void Reset();


private:
	bool leftright;	// false = left,  true = right
	


};





#endif




