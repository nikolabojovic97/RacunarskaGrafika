#pragma once
#define EPS 10e-8

class Vector4D
{
public:
	Vector4D();
	Vector4D(double x, double y, double z);
	Vector4D(double x, double y, double z, double w);
	Vector4D(const Vector4D& v);

	void normalize();
	void normalize3D();
	Vector4D normalized() const;
	Vector4D normalized3D() const;
	double intensity() const;
	double intensity3D() const;
	Vector4D crossProduct(const Vector4D& v) const;
	double angle(const Vector4D& b) const;

	Vector4D operator+(const Vector4D& v) const;
	Vector4D operator-(const Vector4D& v) const;
	Vector4D operator*(double k) const;
	void operator+=(const Vector4D& v);
	void operator-=(const Vector4D& v);
	void operator*=(double k);
	double operator*(const Vector4D& v) const;
	Vector4D operator-() const;

	double& operator[](size_t i);
	double& x();
	double& y();
	double& z();
	double& w();

	void print() const;
public:
	double m_x[4];
};
