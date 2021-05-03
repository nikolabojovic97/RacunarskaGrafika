#include <cmath>
#include <iostream>
#include "Matrix3x3.h"

Matrix3x3::Matrix3x3()
{
	m_mat = new double*[3];
	for (size_t i = 0; i < 3; i++)
	{
		m_mat[i] = new double[3];
		for (size_t j = 0; j < 3; j++)
		{
			m_mat[i][j] = 0.0;
		}
	}
}

Matrix3x3::Matrix3x3(const Matrix3x3& m)
{
	m_mat = new double*[3];
	for (size_t i = 0; i < 3; i++)
	{
		m_mat[i] = new double[3];
		for (size_t j = 0; j < 3; j++)
		{
			m_mat[i][j] = m.m_mat[i][j];
		}
	}
}

void Matrix3x3::toZero()
{
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			m_mat[i][j] = 0.0;
		}
	}
}

void Matrix3x3::toIdentity()
{
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			m_mat[i][j] = i == j ? 1.0 : 0.0;
		}
	}
}

void Matrix3x3::abs()
{
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			m_mat[i][j] = fabs(m_mat[i][j]);
		}
	}
}

void Matrix3x3::transpose()
{
	double t;

	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			t = m_mat[i][j];
			m_mat[i][j] = m_mat[j][i];
			m_mat[j][i] = t;
		}
	}
}

void Matrix3x3::loadTranslate(const Vector2D& v)
{
	loadTranslate(v.m_x[0], v.m_x[1]);
}

void Matrix3x3::loadTranslate(const Vector3D& v)
{
	loadTranslate(v.m_x[0], v.m_x[1]);
}

void Matrix3x3::loadTranslate(double tx, double ty)
{
	toIdentity();
	m_mat[0][2] = tx;
	m_mat[1][2] = ty;
	m_mat[2][2] = 1.0;
}

void Matrix3x3::loadScale(double sx, double sy)
{
	toZero();
	m_mat[0][0] = sx;
	m_mat[1][1] = sy;
	m_mat[2][2] = 1.0;
}

void Matrix3x3::loadRotate(const Vector3D& origin, double phi)
{
	Matrix3x3 tranPos, tranNeg, rot;
	tranPos.loadTranslate(origin);
	tranNeg.loadTranslate(-origin);
	rot.loadRotate(phi);
	*this = tranPos * rot * tranNeg;
}

void Matrix3x3::loadRotate(double phi)
{
	toIdentity();
	m_mat[0][0] = cos(phi);
	m_mat[1][1] = cos(phi);
	m_mat[0][1] = -sin(phi);
	m_mat[1][0] = sin(phi);
	m_mat[2][2] = 1.0;
}

void Matrix3x3::transform(Vector3D& v) const
{
	v = (*this) * v;
}

void Matrix3x3::transform(std::vector<Vector3D>& vectors) const
{
	for (size_t i = 0; i < vectors.size(); i++)
	{
		vectors[i] = (*this) * vectors[i];
	}
}

Vector3D Matrix3x3::transformed(const Vector3D& v) const
{
	Vector3D v_t = v;
	transform(v_t);
	return v_t;
}

std::vector<Vector3D> Matrix3x3::transformed(const std::vector<Vector3D>& vectors) const
{
	std::vector<Vector3D> vectors_t = vectors;
	transform(vectors_t);
	return vectors_t;
}

double Matrix3x3::det() const
{
	return det3x3(
		m_mat[0][0], m_mat[0][1], m_mat[0][2],
		m_mat[1][0], m_mat[1][1], m_mat[1][2],
		m_mat[2][0], m_mat[2][1], m_mat[2][2]
	);
}

double Matrix3x3::det3x3(double a1, double a2, double a3, double b1, double b2, double b3, double c1, double c2, double c3)
{
	return (
		a1 * det2x2(b2, b3, c2, c3) -
		b1 * det2x2(a2, a3, c2, c3) +
		c1 * det2x2(a2, a3, b2, b3)
		);
}

double Matrix3x3::det2x2(double a, double b, double c, double d)
{
	return a * d - b * c;
}

Matrix3x3 Matrix3x3::operator+(const Matrix3x3& m) const
{
	Matrix3x3 n;

	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			n[i][j] = m_mat[i][j] + m.m_mat[i][j];
		}
	}

	return n;
}

Matrix3x3 Matrix3x3::operator-(const Matrix3x3& m) const
{
	Matrix3x3 n;

	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			n[i][j] = m_mat[i][j] - m.m_mat[i][j];
		}
	}

	return n;
}

Matrix3x3 Matrix3x3::operator*(const Matrix3x3& m) const
{
	Matrix3x3 n;

	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			n[i][j] = 0.0;
			for (size_t k = 0; k < 3; k++)
			{
				n[i][j] += m_mat[i][k] * m.m_mat[k][j];
			}
		}
	}

	return n;
}

Vector3D Matrix3x3::operator*(const Vector3D& v) const
{
	Vector3D w;

	for (size_t i = 0; i < 3; i++)
	{
		w[i] = 0.0;
		for (size_t k = 0; k < 3; k++)
		{
			w[i] += m_mat[i][k] * v.m_x[k];
		}
	}

	return w;
}

void Matrix3x3::operator+=(const Matrix3x3& m)
{
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			m_mat[i][j] += m.m_mat[i][j];
		}
	}
}

void Matrix3x3::operator-=(const Matrix3x3& m)
{
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			m_mat[i][j] -= m.m_mat[i][j];
		}
	}
}

void Matrix3x3::operator*=(double k)
{
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			m_mat[i][j] *= k;
		}
	}
}

Matrix3x3 Matrix3x3::operator*(double k) const
{
	Matrix3x3 n;

	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			n[i][j] = m_mat[i][j] * k;
		}
	}

	return n;
}

Matrix3x3 Matrix3x3::operator-() const
{
	Matrix3x3 n;

	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			n[i][j] = -m_mat[i][j];
		}
	}

	return n;
}

bool Matrix3x3::operator==(const Matrix3x3& m) const
{
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			if (fabs(m_mat[i][j] - m.m_mat[i][j]) > EPS)
			{
				return false;
			}
		}
	}

	return true;
}

bool Matrix3x3::operator!=(const Matrix3x3& m) const
{
	return !((*this) == m);
}

double* Matrix3x3::operator[](size_t i)
{
	return m_mat[i];
}

void Matrix3x3::print() const
{
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			std::cout << m_mat[i][j] << " ";
		}
		std::cout << "\n";
	}
}
