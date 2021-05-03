#pragma once
#define EPS 10e-8

class Vector2D
{
public:
	Vector2D();
	Vector2D(double x, double y);
	Vector2D(const Vector2D& v);

	void normalize();
	Vector2D normalized() const;
	double intensity() const;
	double crossProductIntensity(const Vector2D& v) const;
	Vector2D projectionOn(const Vector2D& b) const;
	double angle(const Vector2D& b) const;

	Vector2D operator+(const Vector2D& v) const;
	Vector2D operator-(const Vector2D& v) const;
	Vector2D operator*(double k) const;
	void operator+=(const Vector2D& v);
	void operator-=(const Vector2D& v);
	void operator*=(double k);
	double operator*(const Vector2D& v) const;
	Vector2D operator-() const;

	double& operator[](size_t i);
	double& x();
	double& y();

	void print() const;
public:
	double m_x[2];
};
