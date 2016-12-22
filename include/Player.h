#pragma once
#include "Types.h"


class Player
{
public:
	Player();
	~Player();

	fvec3 getPosition();
	fvec3 getVelocity();
	float getHorizontalSpeed();
	void setPosition(fvec3);
	void setVelocity(fvec3);
	void setRotation(float xAngle);
	BoundingBox getBB();

	bool checkCollision(fvec3 point);

	fvec3 getCameraPosition();

private:
	BoundingBox box;
	fvec3 velocity;


};

