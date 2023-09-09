#pragma once
#include <corecrt_math.h>
#define M_PI 3.14159265358979323846


class Vector
{
public:
	float x, y, z;

	Vector(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}


	Vector()
	{
		this->x = 0;
		this->y = 0;
		this->z = 0;
	}

	inline float Distance(Vector v)
	{
		return sqrt(pow(v.x - this->x, 2) + pow(v.y - this->y, 2) + pow(v.z - this->z, 2));
	}

	inline void AngleVector(Vector& V)
	{
		Vector ret;
		float sp, sy, cp, cy;

		sy = sin(this->y * (M_PI / 180));
		cy = cos(this->y * (M_PI / 180));

		sp = sin(this->x * (M_PI / 180));
		cp = cos(this->x * (M_PI / 180));

		V.x = cp * cy;
		V.y = cp * sy;
		V.z = -sp;
	}

	inline float Length2D()
	{
		return sqrt(pow(this->x, 2) + pow(this->y, 2));
	}
	

	inline void NormalizeInPlace()
	{
		float radius = sqrtf(this->x * this->x + this->y * this->y + this->z * this->z);

		if (radius != 0.0f)
		{
			this->x /= radius;
			this->y /= radius;
			this->z /= radius;
		}
		else
		{
			this->x = this->y = 0.0f; this->z = 1.0f;
		}
	}


	inline float Dot(Vector V)
	{
		return (this->x * V.x + this->y * V.y + this->z * V.z);
	}

	inline float NormalizeYaw(float yaw) {
		yaw = fmod(yaw + 180.0f, 360.0f);
		if (yaw < 0) yaw += 360.0f;
		return yaw - 180.0f;
	}




private:

};



class ViewMatrix {
private:
	float m[4][4];

public:
	// Constructor: Initialize as identity matrix

	inline float* operator[](int i)
	{
		return m[i];
	}

	inline const float* operator[](int i) const
	{
		return m[i];
	}

};

