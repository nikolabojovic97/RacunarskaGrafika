#pragma once
#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <iostream>
using namespace std;

class Vector2D
{
public:
	Vector2D();
	Vector2D(double x, double y) { Set(x, y); }
	Vector2D(const Vector2D &A);

	void Set(double x, double y) { m_x = x; m_y = y; }
	void Init(double x, double y) { Set(x, y); }
	void Copy(Vector2D &A) { m_x = A.m_x; m_y = A.m_y; }

	double& X() { return m_x; }
	double& Y() { return m_y; }

	Vector2D Normalize();

	Vector2D operator +(Vector2D &A);
	Vector2D operator -(Vector2D &A);
	double operator *(Vector2D &A);
	void operator += (Vector2D &A);
	double CrossProductIntensity(Vector2D &A);

	void operator *=(double k);
	Vector2D operator *(double k);

	double Intensity();

	double ProjectionIntensity(Vector2D &b);
	Vector2D ProjectionVector(Vector2D &b);

	friend ostream& operator<< (ostream& out, const Vector2D &a);

public:
	double m_x, m_y;
};

#endif // VECTOR2D_H