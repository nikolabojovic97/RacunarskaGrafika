#include <cmath>
#include <iostream>
#include "Vector2D.h"

Vector2D::Vector2D() : Vector2D(0.0, 0.0)
{
}

Vector2D::Vector2D(double x, double y)
{
	m_x[0] = x;
	m_x[1] = y;
}

Vector2D::Vector2D(const Vector2D& v) : Vector2D(v.m_x[0], v.m_x[1])
{
}

void Vector2D::normalize()
{
	double intensity_ = intensity();

	if (fabs(intensity_) > EPS)
	{
		x() /= intensity_;
		y() /= intensity_;
	}
}

Vector2D Vector2D::normalized() const
{
	Vector2D unit;

	double intensity_ = intensity();

	if (fabs(intensity_) > EPS)
	{
		unit.x() = m_x[0] / intensity_;
		unit.y() = m_x[1] / intensity_;
	}

	return unit;
}

double Vector2D::intensity() const
{
	return sqrt(
		m_x[0] * m_x[0] +
		m_x[1] * m_x[1]
	);
}

double Vector2D::crossProductIntensity(const Vector2D& v) const
{
	return
		m_x[0] * v.m_x[1] -
		m_x[1] * v.m_x[0];
}

Vector2D Vector2D::projectionOn(const Vector2D& b) const
{
	return b.normalized() * intensity() * cos(angle(b));
}

double Vector2D::angle(const Vector2D& b) const
{
	Vector2D a = *this;

	if (a.intensity() < EPS || b.intensity() < EPS)
		return 0.0;

	double cosPhi = (a * b) / (a.intensity() * b.intensity());
	double sinPhi = a.crossProductIntensity(b) / (a.intensity() * b.intensity());

	return atan2(sinPhi, cosPhi);
}

Vector2D Vector2D::operator+(const Vector2D& v) const
{
	return Vector2D(
		m_x[0] + v.m_x[0],
		m_x[1] + v.m_x[1]
	);
}

Vector2D Vector2D::operator-(const Vector2D& v) const
{
	return Vector2D(
		m_x[0] - v.m_x[0],
		m_x[1] - v.m_x[1]
	);
}

Vector2D Vector2D::operator*(double k) const
{
	return Vector2D(
		m_x[0] * k,
		m_x[1] * k
	);
}

void Vector2D::operator+=(const Vector2D& v)
{
	x() += v.m_x[0];
	y() += v.m_x[1];
}

void Vector2D::operator-=(const Vector2D& v)
{
	x() -= v.m_x[0];
	y() -= v.m_x[1];
}

void Vector2D::operator*=(double k)
{
	x() *= k;
	y() *= k;
}

double Vector2D::operator*(const Vector2D& v) const
{
	return
		m_x[0] * v.m_x[0] +
		m_x[1] * v.m_x[1];
}

Vector2D Vector2D::operator-() const
{
	return Vector2D(-m_x[0], -m_x[1]);
}

double& Vector2D::operator[](size_t i)
{
	return m_x[i];
}

double& Vector2D::x()
{
	return m_x[0];
}

double& Vector2D::y()
{
	return m_x[1];
}

void Vector2D::print() const
{
	std::cout << "(" << m_x[0] << ", " << m_x[1] << ")\n";
}
