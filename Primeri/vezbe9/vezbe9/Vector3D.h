#pragma once
#define EPS 10e-8

class Vector3D
{
public:
	Vector3D();
	Vector3D(double x, double y);
	Vector3D(double x, double y, double z);
	Vector3D(const Vector3D& v);

	void normalize();
	Vector3D normalized() const;
	double intensity() const;
	Vector3D crossProduct(const Vector3D& v) const;
	double angle(const Vector3D& b) const;

	Vector3D operator+(const Vector3D& v) const;
	Vector3D operator-(const Vector3D& v) const;
	Vector3D operator*(double k) const;
	void operator+=(const Vector3D& v);
	void operator-=(const Vector3D& v);
	void operator*=(double k);
	double operator*(const Vector3D& v) const;
	Vector3D operator-() const;

	double& operator[](size_t i);
	double& x();
	double& y();
	double& z();

	void print() const;
public:
	double m_x[3];
};

