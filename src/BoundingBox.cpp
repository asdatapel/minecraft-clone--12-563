#include "BoundingBox.h"
#include <algorithm> 


BoundingBox::BoundingBox()
{
}


BoundingBox::~BoundingBox()
{
}

void BoundingBox::setPosition(fvec3 newPosition)
{
	position = newPosition;
}

fvec3 BoundingBox::getPosition()
{
	return position;
}

void BoundingBox::setSize(fvec3 newSize)
{
	size = newSize;
	originOffset = { size.x / 2, size.z / 2,size.y / 2 };
}

fvec3 BoundingBox::getSize()
{
	return size;
}

void BoundingBox::setRotation(fvec3 newRotation)
{
	rotation = newRotation;
	rot = glm::eulerAngleYXZ(rotation.x, rotation.y, rotation.z);
}

void BoundingBox::setRotationX(float newRotation)
{
	rotation.x = newRotation;
	rot = glm::eulerAngleYXZ(rotation.x, rotation.y, rotation.z);
}

void BoundingBox::setRotationZ(float newRotation)
{
	rotation.z = newRotation;
	rot = glm::eulerAngleYXZ(rotation.x, rotation.y, rotation.z);
}
void BoundingBox::setRotationY(float newRotation)
{
	rotation.y = newRotation;
	rot = glm::eulerAngleYXZ(rotation.x, rotation.y, rotation.z);
}

void BoundingBox::changeRotation(fvec3 change) {
	rotation += change;
	rot = glm::eulerAngleYXZ(rotation.y, rotation.x, rotation.z);
}

fvec3 BoundingBox::getRotation()
{
	return rotation;
}

AABB BoundingBox::getAABB()
{
	fvec3 vert1 = position + rotatePoint({ originOffset.x, originOffset.z, originOffset.y });
	fvec3 vert2 = position + rotatePoint({ -originOffset.x, originOffset.z, originOffset.y });
	fvec3 vert3 = position + rotatePoint({ originOffset.x, -originOffset.z, originOffset.y });
	fvec3 vert4 = position + rotatePoint({ originOffset.x, originOffset.z, -originOffset.y });
	fvec3 vert5 = position + rotatePoint({ -originOffset.x, -originOffset.z, originOffset.y });
	fvec3 vert6 = position + rotatePoint({ -originOffset.x, originOffset.z, -originOffset.y });
	fvec3 vert7 = position + rotatePoint({ originOffset.x, -originOffset.z, -originOffset.y });
	fvec3 vert8 = position + rotatePoint({ -originOffset.x, -originOffset.z, -originOffset.y });

	fvec3 min;
	min.x = std::min({ vert1.x, vert2.x, vert3.x, vert4.x, vert5.x, vert6.x, vert7.x, vert8.x });
	min.z = std::min({ vert1.z, vert2.z, vert3.z, vert4.z, vert5.z, vert6.z, vert7.z, vert8.z });
	min.y = std::min({ vert1.y, vert2.y, vert3.y, vert4.y, vert5.y, vert6.y, vert7.y, vert8.y });

	fvec3 max;
	max.x = std::max({ vert1.x, vert2.x, vert3.x, vert4.x, vert5.x, vert6.x, vert7.x, vert8.x });
	max.z = std::max({ vert1.z, vert2.z, vert3.z, vert4.z, vert5.z, vert6.z, vert7.z, vert8.z });
	max.y = std::max({ vert1.y, vert2.y, vert3.y, vert4.y, vert5.y, vert6.y, vert7.y, vert8.y });

	AABB b;
	b.setPosition(min);
	b.setSize(max - min);

	return b;
}

fvec3 BoundingBox::rotatePoint(fvec3 point) {
	glm::vec4 p = { point.x, point.y, point.z , 1.0f};
	p = rot * p;
	return{ p.x, p.z, p.y };
}

bool BoundingBox::checkCollision(fvec3 point)
{
	fvec3 p = point;
	point = rotatePoint(point - position + fvec3{.5f, .5f, .5f});
	p = point;
	point = fvec3{ std::abs(point.x), std::abs(point.z), std::abs(point.y) } - (originOffset);
	p = point;

	//if (pow(point.x, 2) + pow(point.z, 2) + pow(point.y,2) < 0.5f)
	if (point.x < 0.5f && point.z < 0.5f && point.y < 0.5f )
		return true;
	return false;
}
