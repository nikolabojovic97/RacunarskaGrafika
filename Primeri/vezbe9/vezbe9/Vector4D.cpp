#include <cmath>
#include <iostream>
#include "Vector4D.h"

Vector4D::Vector4D() : Vector4D(0.0, 0.0, 0.0, 1.0)
{
}

Vector4D::Vector4D(double x, double y, double z) : Vector4D(x, y, z, 1.0)
{
}

Vector4D::Vector4D(double x, double y, double z, double w)
{
	m_x[0] = x;
	m_x[1] = y;
	m_x[2] = z;
	m_x[3] = w;
}

Vector4D::Vector4D(const Vector4D& v) : Vector4D(v.m_x[0], v.m_x[1], v.m_x[2], v.m_x[3])
{
}

void Vector4D::normalize()
{
	double intensity_ = intensity();

	if (fabs(intensity_) > EPS)
	{
		x() /= intensity_;
		y() /= intensity_;
		z() /= intensity_;
		w() /= intensity_;
	}
}

void Vector4D::normalize3D()
{
	double intensity_ = intensity3D();

	if (fabs(intensity_) > EPS)
	{
		x() /= intensity_;
		y() /= intensity_;
		z() /= intensity_;
	}

	w() = 1.0;
}

Vector4D Vector4D::normalized() const
{
	Vector4D unit;

	double intensity_ = intensity();

	if (fabs(intensity_) > EPS)
	{
		unit.x() = m_x[0] / intensity_;
		unit.y() = m_x[1] / intensity_;
		unit.z() = m_x[2] / intensity_;
		unit.w() = m_x[3] / intensity_;
	}

	return unit;
}

Vector4D Vector4D::normalized3D() const
{
	Vector4D unit;

	double intensity_ = intensity3D();

	if (fabs(intensity_) > EPS)
	{
		unit.x() = m_x[0] / intensity_;
		unit.y() = m_x[1] / intensity_;
		unit.z() = m_x[2] / intensity_;
	}

	return unit;
}

double Vector4D::intensity() const
{
	return sqrt(
		m_x[0] * m_x[0] +
		m_x[1] * m_x[1] +
		m_x[2] * m_x[2] +
		m_x[3] * m_x[3]
	);
}

double Vector4D::intensity3D() const
{
	return sqrt(
		m_x[0] * m_x[0] +
		m_x[1] * m_x[1] +
		m_x[2] * m_x[2]
	);
}

Vector4D Vector4D::crossProduct(const Vector4D& v) const
{
	return Vector4D(
		m_x[1] * v.m_x[2] - m_x[2] * v.m_x[1],
		m_x[2] * v.m_x[0] - m_x[0] * v.m_x[2],
		m_x[0] * v.m_x[1] - m_x[1] * v.m_x[0],
		1.0
	);
}

double Vector4D::angle(const Vector4D& b_orig) const
{
	Vector4D a = *this;
	a.m_x[3] = 0.0;
	Vector4D b = b_orig;
	b.m_x[3] = 0.0;
	Vector4D axb = a.crossProduct(b);
	axb.m_x[3] = 0.0;

	if (a.intensity3D() < EPS || b.intensity3D() < EPS)
		return 0.0;

	double cosPhi = (a * b) / (a.intensity3D() * b.intensity3D());
	double sinPhi = axb.intensity3D() / (a.intensity3D() * b.intensity3D());

	return atan2(sinPhi, cosPhi);
}

Vector4D Vector4D::operator+(const Vector4D& v) const
{
	return Vector4D(
		m_x[0] + v.m_x[0],
		m_x[1] + v.m_x[1],
		m_x[2] + v.m_x[2],
		m_x[3] + v.m_x[3]
	);
}

Vector4D Vector4D::operator-(const Vector4D& v) const
{
	return Vector4D(
		m_x[0] - v.m_x[0],
		m_x[1] - v.m_x[1],
		m_x[2] - v.m_x[2],
		m_x[3] - v.m_x[3]
	);
}

Vector4D Vector4D::operator*(double k) const
{
	return Vector4D(
		m_x[0] * k,
		m_x[1] * k,
		m_x[2] * k,
		m_x[3] * k
	);
}

void Vector4D::operator+=(const Vector4D& v)
{
	x() += v.m_x[0];
	y() += v.m_x[1];
	z() += v.m_x[2];
	w() += v.m_x[3];
}

void Vector4D::operator-=(const Vector4D& v)
{
	x() -= v.m_x[0];
	y() -= v.m_x[1];
	z() -= v.m_x[2];
	w() -= v.m_x[3];
}

void Vector4D::operator*=(double k)
{
	x() *= k;
	y() *= k;
	z() *= k;
	w() *= k;
}

double Vector4D::operator*(const Vector4D& v) const
{
	return
		m_x[0] * v.m_x[0] +
		m_x[1] * v.m_x[1] +
		m_x[2] * v.m_x[2] +
		m_x[3] * v.m_x[3];
}

Vector4D Vector4D::operator-() const
{
	return Vector4D(-m_x[0], -m_x[1], -m_x[2], -m_x[3]);
}

double& Vector4D::operator[](size_t i)
{
	return m_x[i];
}

double& Vector4D::x()
{
	return m_x[0];
}

double& Vector4D::y()
{
	return m_x[1];
}

double& Vector4D::z()
{
	return m_x[2];
}

double& Vector4D::w()
{
	return m_x[3];
}

void Vector4D::print() const
{
	std::cout << "(" << m_x[0] << ", " << m_x[1] << ", " << m_x[2] << ", " << m_x[3] << ")\n";
}