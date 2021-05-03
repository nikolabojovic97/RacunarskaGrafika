#include <cmath>
#include <iostream>
#include "Vector3D.h"

Vector3D::Vector3D() : Vector3D(0.0, 0.0, 0.0)
{
}

Vector3D::Vector3D(double x, double y) : Vector3D(x, y, 1.0)
{
}

Vector3D::Vector3D(double x, double y, double z)
{
	m_x[0] = x;
	m_x[1] = y;
	m_x[2] = z;
}

Vector3D::Vector3D(const Vector3D& v) : Vector3D(v.m_x[0], v.m_x[1], v.m_x[2])
{
}

void Vector3D::normalize()
{
	double intensity_ = intensity();

	if (fabs(intensity_) > EPS)
	{
		x() /= intensity_;
		y() /= intensity_;
		z() /= intensity_;
	}
}

Vector3D Vector3D::normalized() const
{
	Vector3D unit;

	double intensity_ = intensity();

	if (fabs(intensity_) > EPS)
	{
		unit.x() = m_x[0] / intensity_;
		unit.y() = m_x[1] / intensity_;
		unit.z() = m_x[2] / intensity_;
	}

	return unit;
}

double Vector3D::intensity() const
{
	return sqrt(
		m_x[0] * m_x[0] +
		m_x[1] * m_x[1] +
		m_x[2] * m_x[2]
	);
}

Vector3D Vector3D::crossProduct(const Vector3D& v) const
{
	return Vector3D(
		m_x[1] * v.m_x[2] - m_x[2] * v.m_x[1],
		m_x[2] * v.m_x[0] - m_x[0] * v.m_x[2],
		m_x[0] * v.m_x[1] - m_x[1] * v.m_x[0]
	);
}

double Vector3D::angle(const Vector3D& b) const
{
	Vector3D a = *this;

	if (a.intensity() < EPS || b.intensity() < EPS)
		return 0.0;

	double cosPhi = (a * b) / (a.intensity() * b.intensity());
	double sinPhi = a.crossProduct(b).intensity() / (a.intensity() * b.intensity());

	return atan2(sinPhi, cosPhi);
}

Vector3D Vector3D::operator+(const Vector3D& v) const
{
	return Vector3D(
		m_x[0] + v.m_x[0],
		m_x[1] + v.m_x[1],
		m_x[2] + v.m_x[2]
	);
}

Vector3D Vector3D::operator-(const Vector3D& v) const
{
	return Vector3D(
		m_x[0] - v.m_x[0],
		m_x[1] - v.m_x[1],
		m_x[2] - v.m_x[2]
	);
}

Vector3D Vector3D::operator*(double k) const
{
	return Vector3D(
		m_x[0] * k,
		m_x[1] * k,
		m_x[2] * k
	);
}

void Vector3D::operator+=(const Vector3D& v)
{
	x() += v.m_x[0];
	y() += v.m_x[1];
	z() += v.m_x[2];
}

void Vector3D::operator-=(const Vector3D& v)
{
	x() -= v.m_x[0];
	y() -= v.m_x[1];
	z() -= v.m_x[2];
}

void Vector3D::operator*=(double k)
{
	x() *= k;
	y() *= k;
	z() *= k;
}

double Vector3D::operator*(const Vector3D& v) const
{
	return
		m_x[0] * v.m_x[0] +
		m_x[1] * v.m_x[1] +
		m_x[2] * v.m_x[2];
}

Vector3D Vector3D::operator-() const
{
	return Vector3D(-m_x[0], -m_x[1], -m_x[2]);
}

double& Vector3D::operator[](size_t i)
{
	return m_x[i];
}

double& Vector3D::x()
{
	return m_x[0];
}

double& Vector3D::y()
{
	return m_x[1];
}

double& Vector3D::z()
{
	return m_x[2];
}

void Vector3D::print() const
{
	std::cout << "(" << m_x[0] << ", " << m_x[1] << ", " << m_x[2] << ")\n";
}
