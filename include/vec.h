#pragma once
#include <tuple>
#include <math.h>
#include <functional>

struct ivec2 {
	ivec2() {};
	ivec2(int X, int Z):x(X), z(Z) {};
	ivec2(float X, float Z) : x(floor(X)), z(floor(Z)) {};
	int x;
	int z;
};

struct fvec3 {
	fvec3() {};
	fvec3(float X, float Z, float Y) :x(X), y(Y), z(Z) {}
	fvec3(int X, int Z, int Y) :x(X), y(Y), z(Z) {}
	fvec3(double X, double Z, double Y) :x(X), y(Y), z(Z) {}


	float x;
	float z;
	float y;

	fvec3 normalize();
	float length();
};

bool operator < (const fvec3& a, const fvec3& b);
bool operator > (const fvec3& a, const fvec3& b);
bool operator <= (const fvec3& a, const fvec3& b);
bool operator >= (const fvec3& a, const fvec3& b);
bool operator == (const fvec3& a, const fvec3& b);


bool operator == (const ivec2 a, const ivec2& b);


fvec3& operator += (fvec3& a, int b);
fvec3& operator += (fvec3& a, const fvec3& b);
fvec3& operator *= (fvec3& a, int b);
fvec3& operator *= (fvec3& a, const fvec3& b);

fvec3 operator + (const fvec3& a, float b);
fvec3 operator + (const fvec3& a, const fvec3& b);
fvec3 operator - (const fvec3& a, float b);
fvec3 operator - (const fvec3& a, const fvec3& b);
fvec3 operator * (const fvec3& a, float b);
fvec3 operator * (const fvec3& a, const fvec3& b);
fvec3 operator / (const fvec3& a, float b);
fvec3 operator / (const fvec3& a, const fvec3& b);


ivec2& operator += (ivec2& a, const ivec2& b);
ivec2 operator + (const ivec2& a, const ivec2& b);

fvec3 floor(fvec3 vec);


namespace std
{
	template <>
	struct hash<ivec2>
	{
		size_t operator()(ivec2 const & x) const noexcept
		{
			return (
				(51 + std::hash<int>()(x.x)) * 51
				+ std::hash<int>()(x.z)
				);
		}
	};
}