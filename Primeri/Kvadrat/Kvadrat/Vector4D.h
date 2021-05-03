#ifndef VECTOR4D_H
#define VECTOR4D_H

#include <ostream>

using namespace std;

#define EPSILON 0.1e-10


class Vector4D
{
public:
	double x;
	double y;
	double z;
	double t;
public:
	Vector4D();
	Vector4D(double x, double y, double z, double t);
	Vector4D(Vector4D& A);
	Vector4D(const Vector4D& A);

	double X() {
		return x;
	}
	double Y() {
		return y;
	}
	double Z() {
		return z;
	}
	double T() {
		return t;
	}

	void Set(double x, double y, double z, double t) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->t = t;
	}
	void Copy(Vector4D& A) {
		Set(A.x, A.y, A.z, A.t);
	}
	void Zero() {
		x = y = z = t = 0;
	}

	Vector4D operator +(Vector4D& A);
	void operator +=(Vector4D& A);
	Vector4D operator -(Vector4D& A);
	void operator -=(Vector4D& A);
	double operator *(Vector4D& A);
	double CrossProductIntensity(Vector4D& A);
	Vector4D CrossProduct3D(Vector4D& A);

	Vector4D operator *(double k);
	void operator *=(double k);

	Vector4D Normalize();
	double Intensity();

	double ProjectionIntensity(Vector4D& A);
	Vector4D ProjectionVector(Vector4D& A);

	double AngleBetween(Vector4D& A);

	friend ostream& operator <<(ostream& out, const Vector4D& A);
};

#endif 