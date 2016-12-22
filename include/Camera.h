#pragma once
#include <iostream>

#include "Types.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>

class Camera
{
public:
	Camera();
	~Camera();

	void rotate(float x, float y);
	void update();
	glm::mat4 getPVMat(); 
	glm::mat4 getCenteredPVMat();
	void move(fvec3 distance);
	fvec3 getForwardVector();
	fvec3 getRightVector();
	fvec3 getDirectionVector();
	fvec3 getDirectionAngles();

	void setPosition(fvec3 newPos);
	fvec3 getPosition();


private:
	glm::vec3 direction;
	glm::vec3 forward;
	glm::vec3 position;
	glm::vec3 right;
	glm::vec3 up;
	float horizontalAngle;
	float verticalAngle;
	float initialFoV;
	float speed;
	float sensitivity;

	glm::mat4 view;
	glm::mat4 proj;

};

