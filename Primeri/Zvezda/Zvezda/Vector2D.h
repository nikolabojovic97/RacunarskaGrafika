#pragma once
#include <ostream>

#ifndef VECTOR2D_H
#define VECTOR2D_H

using namespace std;

class Vector2D
{
public:
	double x;
	double y;

public:
	Vector2D();
	Vector2D(double x, double y) {
		Set(x, y);
	}
	Vector2D(const Vector2D& A);

	void Set(double x, double y) {
		this->x = x;
		this->y = y;
	}
	void Init(double x, double y) {
		Set(x, y);
	}
	void Copy(Vector2D& A) {
		Set(A.x, A.y);
	}

	Vector2D operator +(Vector2D& A);
	Vector2D operator -(Vector2D& A);
	double operator *(Vector2D& A);
	void operator +=(Vector2D& A);
	double CrossProductIntensity(Vector2D& A);

	void operator *=(double k);
	Vector2D operator *(double k);

	double Intensity();
	Vector2D Normalize();
	Vector2D Transpose();

	double ProjectionIntensity(Vector2D& A);
	Vector2D ProjectionVector(Vector2D& A);

	friend ostream& operator <<(ostream& out, const Vector2D& A);
};

#endif // !VECTOR2D_H