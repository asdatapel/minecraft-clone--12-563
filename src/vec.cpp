#include "vec.h"


bool operator < (const fvec3& a, const fvec3& b) {
	return std::tie(a.x, a.z, a.y) < std::tie(b.x, b.z, b.y);
}

bool operator > (const fvec3& a, const fvec3& b) {
	return std::tie(a.x, a.z, a.y) > std::tie(b.x, b.z, b.y);
}
bool operator <= (const fvec3& a, const fvec3& b) {
	return std::tie(a.x, a.z, a.y) <= std::tie(b.x, b.z, b.y);
}
bool operator >= (const fvec3& a, const fvec3& b) {
	return std::tie(a.x, a.z, a.y) >= std::tie(b.x, b.z, b.y);
}
bool operator == (const fvec3& a, const fvec3& b) {
	return std::tie(a.x, a.z, a.y) == std::tie(b.x, b.z, b.y);
}

bool operator == (const ivec2 a, const ivec2& b) {
	return std::tie(a.x, a.z) == std::tie(b.x, b.z);
}


fvec3& operator += (fvec3& a, float b) {
	a.x += b;
	a.z += b;
	a.y += b;

	return a;
}
fvec3& operator += (fvec3& a, const fvec3& b) {
	a.x += b.x;
	a.z += b.z;
	a.y += b.y;

	return a;
}
fvec3& operator *= (fvec3& a, float b) {
	a.x *= b;
	a.z *= b;
	a.y *= b;

	return a;
}
fvec3& operator *= (fvec3& a, const fvec3& b) {
	a.x *= b.x;
	a.z *= b.z;
	a.y *= b.y;

	return a;
}

fvec3 operator + (const fvec3& a, float b) {
	return{ a.x + b, a.z + b, a.y + b };
}
fvec3 operator + (const fvec3& a, const fvec3& b) {
	return{ a.x + b.x, a.z + b.z, a.y + b.y };
}
fvec3 operator - (const fvec3& a, float b) {
	return{ a.x - b, a.z - b, a.y - b };
}
fvec3 operator - (const fvec3& a, const fvec3& b) {
	return{ a.x - b.x, a.z - b.z, a.y - b.y };
}
fvec3 operator * (const fvec3& a, float b) {
	return{ a.x * b, a.z * b, a.y * b };
}
fvec3 operator * (const fvec3& a, const fvec3& b) {
	return{ a.x * b.x, a.z * b.z, a.y * b.y };
}
fvec3 operator / (const fvec3& a, float b) {
	return{ (float)a.x / b, (float)a.z / b, (float)a.y / b };
}
fvec3 operator / (const fvec3& a, const fvec3& b) {
	return{ a.x / b.x, a.z / b.z, a.y / b.y };
}


ivec2& operator += (ivec2& a, const ivec2& b) {
	a.x += b.x;
	a.z += b.z;

	return a;
}

ivec2 operator + (const ivec2& a, const ivec2& b) {
	return{ a.x + b.x,	a.z + b.z };
}

fvec3 floor(fvec3 vec) {
	return { floor(vec.x), floor(vec.z), floor(vec.y) };
}

fvec3 fvec3::normalize() {
	float mag = length();
	return {x / mag, z / mag, y / mag};
}

float fvec3::length() {
    return sqrt(x*x + z*z + y*y);

}