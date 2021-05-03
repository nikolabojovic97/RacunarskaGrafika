#include "Vector2D.h"
#include <math.h>

using namespace std;
Vector2D::Vector2D()
{
}

Vector2D::Vector2D(const Vector2D &A)
{
	m_x = A.m_x;
	m_y = A.m_y;
}

void Vector2D::operator *=(double k)
{
	m_x *= k;
	m_y *= k;
}

Vector2D Vector2D::operator *(double k)
{
	return(Vector2D(m_x*k, m_y*k));
}

Vector2D Vector2D::operator +(Vector2D &A)
{
	return(Vector2D(m_x + A.m_x, m_y + A.m_y));
}

Vector2D Vector2D::operator -(Vector2D &A)
{
	return(Vector2D(m_x - A.m_x, m_y - A.m_y));
}

void Vector2D::operator +=(Vector2D& A)
{
	m_x += A.m_x;
	m_y += A.m_y;
}

double Vector2D::CrossProductIntensity(Vector2D &A)
{
	return (m_x*A.m_y - m_y * A.m_x);
}

double Vector2D::Intensity()
{
	return sqrt(m_x*m_x + m_y * m_y);
}

double Vector2D::operator *(Vector2D &A)
{
	return m_x * A.X() + m_y * A.Y();
}

double Vector2D::ProjectionIntensity(Vector2D & b)
{
	Vector2D projection = ProjectionVector(b);
	return projection.Intensity();
}

Vector2D Vector2D::ProjectionVector(Vector2D &b)
{
	Vector2D a(m_x, m_y);

	double kosFi = (b * a) / (b.Intensity() * a.Intensity());
	double sinFi = (b.CrossProductIntensity(a)) / (b.Intensity() * a.Intensity());

	double fi = atan2(sinFi, kosFi);

	double intensity = a.Intensity() * cos(fi);

	Vector2D b_unit = b.Normalize();

	return b_unit * intensity;
}

Vector2D Vector2D::Normalize()
{
	double intensity = Intensity();
	Vector2D v(0.0, 0.0);

	if (fabs(intensity) > 1.0e-7)
	{
		v.X() = m_x / intensity;
		v.Y() = m_y / intensity;
	}

	return v;
}

ostream& operator<< (ostream& out, const Vector2D &a)
{
	cout << "(" << a.m_x << "," << a.m_y << ")";

	return out;
}