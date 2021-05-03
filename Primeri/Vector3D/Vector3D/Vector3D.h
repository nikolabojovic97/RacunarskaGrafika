#pragma once
#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <iostream>
using namespace std;

class Vector3D
{
public:
	double x;
	double y;
	double z;

public:
	Vector3D();
	Vector3D(double x, double y, double z);
	Vector3D(const Vector3D& A);

	double X();
	double Y();
	double Z();

	void Set(double x, double y, double z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	void Init(double x, double y, double z) {
		Set(x, y, z);
	}
	void Copy(Vector3D& A) {
		x = A.x;
		y = A.y;
		z = A.z;
	}

	Vector3D Normalize();

	Vector3D operator +(Vector3D& A);
	void operator +=(Vector3D& A);
	Vector3D operator -(Vector3D& A);
	double operator *(Vector3D& A);
	double CrossProductIntensity(Vector3D& A);

	void operator *=(double k);
	Vector3D operator *(double k);

	double Intensity();

	double ProjectionIntensity(Vector3D& A);
	Vector3D ProjectionVector(Vector3D& A);

	friend ostream& operator <<(ostream& out, const Vector3D& A);

};

#endif


