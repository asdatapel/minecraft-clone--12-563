#pragma once
#include "AABB.h"

#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>

class BoundingBox
{
public:
	BoundingBox();
	~BoundingBox();

	void setPosition(fvec3 newPos);
	fvec3 getPosition();
	void setSize(fvec3 newSize);
	fvec3 getSize();
	void setRotation(fvec3 newRotation);
	void setRotationX(float newRotation);
	void setRotationZ(float newRotation);
	void setRotationY(float newRotation);
	void changeRotation(fvec3 change);
	fvec3 getRotation();

	AABB getAABB(); 
	fvec3 rotatePoint(fvec3 point);
	bool checkCollision(fvec3 point);

	fvec3 position;
	fvec3 size;
	fvec3 rotation;
	fvec3 originOffset; 
	glm::mat4 rot;
};

