#ifndef GAMEPLAY_CAM_H
#define GAMEPLAY_CAM_H

#include "Camera3.h"


class GameplayCam : public Camera3
{
public:

	GameplayCam();
	~GameplayCam();
	virtual void Init(const Vector3& pos, const Vector3& target, Vector3& up);
	virtual void Update(double dt);
	virtual void Reset();

	//turntable camera
	void TurnTable(float cam_spd, double dt, Vector3& targetpos);

	//Gameplay panning camera
	void PanCamera(float cam_spd, double dt);


private:
	bool leftright;	// false = left,  true = right
	float turnSpeed_Mod;

	bool started;


};





#endif




