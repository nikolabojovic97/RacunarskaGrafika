#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include "Matrix3x3.h"
#include "Matrix4x4.h"

Matrix4x4::Matrix4x4()
{
	m_mat = new double* [4];
	for (size_t i = 0; i < 4; i++)
	{
		m_mat[i] = new double[4];
		for (size_t j = 0; j < 4; j++)
		{
			m_mat[i][j] = 0.0;
		}
	}
}

Matrix4x4::Matrix4x4(const Matrix4x4& m)
{
	m_mat = new double* [4];
	for (size_t i = 0; i < 4; i++)
	{
		m_mat[i] = new double[4];
		for (size_t j = 0; j < 4; j++)
		{
			m_mat[i][j] = m.m_mat[i][j];
		}
	}
}

void Matrix4x4::toZero()
{
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			m_mat[i][j] = 0.0;
		}
	}
}

void Matrix4x4::toIdentity()
{
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			m_mat[i][j] = i == j ? 1.0 : 0.0;
		}
	}
}

void Matrix4x4::abs()
{
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			m_mat[i][j] = fabs(m_mat[i][j]);
		}
	}
}

void Matrix4x4::transpose()
{
	double t;

	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = i; j < 4; j++)
		{
			t = m_mat[i][j];
			m_mat[i][j] = m_mat[j][i];
			m_mat[j][i] = t;
		}
	}
}

void Matrix4x4::loadTranslate(const Vector3D& v)
{
	loadTranslate(v.m_x[0], v.m_x[1], v.m_x[2]);
}

void Matrix4x4::loadTranslate(const Vector4D& v)
{
	loadTranslate(v.m_x[0], v.m_x[1], v.m_x[2]);
}

void Matrix4x4::loadTranslate(double tx, double ty, double tz)
{
	toIdentity();
	m_mat[0][3] = tx;
	m_mat[1][3] = ty;
	m_mat[2][3] = tz;
}

void Matrix4x4::loadScale(double sx, double sy, double sz)
{
	toZero();
	m_mat[0][0] = sx;
	m_mat[1][1] = sy;
	m_mat[2][2] = sz;
	m_mat[3][3] = 1.0;
}

void Matrix4x4::loadRotateX(double phi)
{
	toIdentity();
	m_mat[1][1] = m_mat[2][2] = cos(phi);
	m_mat[1][2] = -sin(phi);
	m_mat[2][1] = sin(phi);
	m_mat[0][0] = m_mat[3][3] = 1.0;
}

void Matrix4x4::loadRotateY(double phi)
{
	toIdentity();
	m_mat[0][0] = m_mat[2][2] = cos(phi);
	m_mat[0][2] = sin(phi);
	m_mat[2][0] = -sin(phi);
	m_mat[1][1] = m_mat[3][3] = 1.0;
}

void Matrix4x4::loadRotateZ(double phi)
{
	toIdentity();
	m_mat[0][0] = m_mat[1][1] = cos(phi);
	m_mat[1][0] = sin(phi);
	m_mat[0][1] = -sin(phi);
	m_mat[2][2] = m_mat[3][3] = 1.0;
}

void Matrix4x4::loadRotateAxis(const Vector4D& axis, double angle)
{
	loadRotateAxis(axis, angle, Vector4D());
}

void Matrix4x4::loadRotateAxis(const Vector4D& axis, double angle, const Vector4D& origin)
{
	Matrix4x4 tranpos, tranneg, rotYpos, rotYneg, rotXpos, rotXneg, rotZ;
	double teta, phi;
	double d = sqrt(axis.m_x[0] * axis.m_x[0] + axis.m_x[2] * axis.m_x[2]);

	if (fabs(axis.m_x[0]) < EPS)
		teta = 0.0;
	else
	{
		if (fabs(axis.m_x[2]) > EPS) teta = atan(axis.m_x[0] / axis.m_x[2]);
		else
		{
			if (axis.m_x[0] > 0.0) teta = M_PI / 2;
			else teta = -M_PI / 2;
		}
	}

	if (fabs(axis.m_x[1]) < EPS)
		phi = 0.0;
	else
	{
		if (fabs(d) > EPS) phi = atan(axis.m_x[1] / d);
		else
		{
			if (axis.m_x[1] > 0.0) phi = M_PI / 2;
			else phi = -M_PI / 2;
		}
	}

	tranpos.loadTranslate(origin);
	tranneg.loadTranslate(-origin);
	rotYpos.loadRotateY(teta);
	rotYneg.loadRotateY(-teta);
	rotXpos.loadRotateX(phi);
	rotXneg.loadRotateX(-phi);
	rotZ.loadRotateZ(angle);
	(*this) = tranpos * rotYpos * rotXneg * rotZ * rotXpos * rotYneg * tranneg;
}

void Matrix4x4::loadMatchVector(const Vector4D& from, const Vector4D& to)
{
	loadMatchVector(from, to, Vector4D());
}

void Matrix4x4::loadMatchVector(const Vector4D& from, const Vector4D& to, const Vector4D& origin)
{
	Vector4D axis = from.crossProduct(to);
	loadRotateAxis(axis, -from.angle(to), origin);
}

void Matrix4x4::transform(Vector4D& v) const
{
	v = (*this) * v;
}

void Matrix4x4::transform(std::vector<Vector4D>& vectors) const
{
	for (size_t i = 0; i < vectors.size(); i++)
	{
		vectors[i] = (*this) * vectors[i];
	}
}

Vector4D Matrix4x4::transformed(const Vector4D& v)
{
	Vector4D v_t = v;
	transform(v_t);
	return v_t;
}

std::vector<Vector4D> Matrix4x4::transformed(const std::vector<Vector4D>& vectors)
{
	std::vector<Vector4D> vectors_t = vectors;
	transform(vectors_t);
	return vectors_t;
}

double Matrix4x4::det() const
{
	double a1, a2, a3, a4, b1, b2, b3, b4, c1, c2, c3, c4, d1, d2, d3, d4;

	a1 = m_mat[0][0];
	b1 = m_mat[1][0];
	c1 = m_mat[2][0];
	d1 = m_mat[3][0];
	a2 = m_mat[0][1];
	b2 = m_mat[1][1];
	c2 = m_mat[2][1];
	d2 = m_mat[3][1];
	a3 = m_mat[0][2];
	b3 = m_mat[1][2];
	c3 = m_mat[2][2];
	d3 = m_mat[3][2];
	a4 = m_mat[0][3];
	b4 = m_mat[1][3];
	c4 = m_mat[2][3];
	d4 = m_mat[3][3];
	return (
		a1 * Matrix3x3::det3x3(b2, b3, b4, c2, c3, c4, d2, d3, d4) -
		b1 * Matrix3x3::det3x3(a2, a3, a4, c2, c3, c4, d2, d3, d4) +
		c1 * Matrix3x3::det3x3(a2, a3, a4, b2, b3, b4, d2, d3, d4) -
		d1 * Matrix3x3::det3x3(a2, a3, a4, b2, b3, b4, c2, c3, c4)
		);
}

Matrix4x4 Matrix4x4::operator+(const Matrix4x4& m) const
{
	Matrix4x4 n;

	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			n[i][j] = m_mat[i][j] + m.m_mat[i][j];
		}
	}

	return n;
}

Matrix4x4 Matrix4x4::operator-(const Matrix4x4& m) const
{
	Matrix4x4 n;

	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			n[i][j] = m_mat[i][j] - m.m_mat[i][j];
		}
	}

	return n;
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& m) const
{
	Matrix4x4 n;

	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			n[i][j] = 0.0;
			for (size_t k = 0; k < 4; k++)
			{
				n[i][j] += m_mat[i][k] * m.m_mat[k][j];
			}
		}
	}

	return n;
}

Matrix4x4 Matrix4x4::operator*(double k) const
{
	Matrix4x4 n;

	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			n[i][j] = m_mat[i][j] * k;
		}
	}

	return n;
}

Vector4D Matrix4x4::operator*(const Vector4D& v) const
{
	Vector4D w;

	for (size_t i = 0; i < 4; i++)
	{
		w[i] = 0.0;
		for (size_t k = 0; k < 4; k++)
		{
			w[i] += m_mat[i][k] * v.m_x[k];
		}
	}

	return w;
}

Matrix4x4 Matrix4x4::operator-() const
{
	Matrix4x4 n;

	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			n[i][j] = -m_mat[i][j];
		}
	}

	return n;
}

void Matrix4x4::operator+=(const Matrix4x4& m)
{
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			m_mat[i][j] += m.m_mat[i][j];
		}
	}
}

void Matrix4x4::operator-=(const Matrix4x4& m)
{
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			m_mat[i][j] -= m.m_mat[i][j];
		}
	}
}

void Matrix4x4::operator*=(double k)
{
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			m_mat[i][j] *= k;
		}
	}
}

bool Matrix4x4::operator==(const Matrix4x4& m) const
{
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			if (fabs(m_mat[i][j] - m.m_mat[i][j]) > EPS)
			{
				return false;
			}
		}
	}

	return true;
}

bool Matrix4x4::operator!=(const Matrix4x4& m) const
{
	return !((*this) == m);
}

double* Matrix4x4::operator[](size_t i)
{
	return m_mat[i];
}

void Matrix4x4::print() const
{
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			std::cout << m_mat[i][j] << " ";
		}
		std::cout << "\n";
	}
}