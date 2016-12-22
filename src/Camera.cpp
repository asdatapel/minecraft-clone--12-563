#include "Camera.h"

Camera::Camera()
{
	direction = glm::vec3(cos(verticalAngle) * sin(horizontalAngle), sin(verticalAngle), cos(verticalAngle) * cos(horizontalAngle));
	position = glm::vec3(0, 0, 5);
	right = glm::vec3(sin(horizontalAngle - 3.14f / 2.0f), 0, cos(horizontalAngle - 3.14f / 2.0f));
	up = glm::cross(right, direction);
	horizontalAngle = 3.14f;
	verticalAngle = 0.0f;
	initialFoV = 45.0f;
	speed = 3.0f;
	sensitivity = 0.005f;

	view = glm::lookAt(position, position + direction, up);
	proj = glm::perspective(glm::radians(70.0f), 1920.0f / 1080.0f, 0.01f, 1000.0f);
}


Camera::~Camera()
{
}

void Camera::rotate(float x, float y) {
	horizontalAngle += sensitivity * x;
	verticalAngle += sensitivity * y;
	if (verticalAngle > (3.14f / 2))
		verticalAngle = (3.15f / 2);
	else if (verticalAngle < (-3.14f / 2))
		verticalAngle = (-3.14f / 2);
}

void Camera::move(fvec3 distance) {
	position += glm::vec3(distance.x, distance.y, distance.z);
}

void Camera::update() {
	direction = glm::normalize(glm::vec3(cos(verticalAngle) * sin(horizontalAngle), sin(verticalAngle), cos(verticalAngle) * cos(horizontalAngle)));

	right = glm::normalize(glm::vec3(sin(horizontalAngle - 3.14f / 2.0f), 0, cos(horizontalAngle - 3.14f / 2.0f)));
	up = glm::normalize(glm::cross(right, direction));
	forward = glm::normalize(glm::vec3(direction.x, 0, direction.z));

	view = glm::lookAt(position, position + direction, up);

}

glm::mat4 Camera::getPVMat() {
	update();
	return proj * view;
}

glm::mat4 Camera::getCenteredPVMat() {
	update();
	return proj *  glm::lookAt({ 0,0,0 }, direction, up);;
}

fvec3 Camera::getPosition() {
	return fvec3(position.x, position.z, position.y);
}

void Camera::setPosition(fvec3 newPos) {
	position = glm::vec3(newPos.x, newPos.y, newPos.z);
}

fvec3 Camera::getForwardVector() {
	return fvec3(forward.x, forward.z, forward.y );
}
fvec3 Camera::getRightVector() {
	return fvec3(right.x, right.z, right.y);
}

fvec3 Camera::getDirectionVector()
{
	return fvec3(direction.x, direction.z, direction.y );
}

fvec3 Camera::getDirectionAngles()
{
	return{ horizontalAngle, 0.0f, verticalAngle };
}
