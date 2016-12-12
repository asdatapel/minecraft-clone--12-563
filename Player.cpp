#include "Player.h"



Player::Player()
{
	box.setSize({ 0.6f, 0.6f, 1.85f });
	box.setRotation({ 0,0,0 });
}


Player::~Player()
{
}

float Player::getHorizontalSpeed() {
	return sqrt(velocity.x * velocity.x + velocity.z * velocity.z);
}

fvec3 Player::getPosition()
{
	return box.getPosition();
}

fvec3 Player::getVelocity()
{
	return velocity;
}

void Player::setPosition(fvec3 pos)
{
	box.setPosition(pos);
}

void Player::setVelocity(fvec3 vel)
{
	velocity = vel;
}

void Player::setRotation(float xAngle)
{
	fvec3 old = box.getRotation();
	box.setRotation({ xAngle, old.z, old.y });
}

BoundingBox Player::getBB()
{
	return box;
}

bool Player::checkCollision(fvec3 point)
{
	return box.checkCollision(point);
}

fvec3 Player::getCameraPosition()
{
	return box.position + fvec3{ 0.0f,0.0f,0.4175f };
}