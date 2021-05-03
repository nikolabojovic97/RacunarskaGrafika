#pragma once
#include <vector>
#include "Vector2D.h"
#include "Vector3D.h"
#define EPS 10e-8

class Matrix3x3
{
public:
	Matrix3x3();
	Matrix3x3(const Matrix3x3& m);

	void toZero();
	void toIdentity();
	void abs();
	void transpose();

	void loadTranslate(const Vector2D& v);
	void loadTranslate(const Vector3D& v);
	void loadTranslate(double tx, double ty);
	void loadScale(double sx, double sy);
	void loadRotate(const Vector3D& origin, double phi);
	void loadRotate(double phi);

	void transform(Vector3D& v) const;
	void transform(std::vector<Vector3D>& vectors) const;
	Vector3D transformed(const Vector3D& v) const;
	std::vector<Vector3D> transformed(const std::vector<Vector3D>& vectors) const;

	double det() const;
	static double det3x3(
		double a1, double a2, double a3,
		double b1, double b2, double b3,
		double c1, double c2, double c3
	);
	static double det2x2(double a, double b, double c, double d);

	Matrix3x3 operator+(const Matrix3x3& m) const;
	Matrix3x3 operator-(const Matrix3x3& m) const;
	Matrix3x3 operator*(const Matrix3x3& m) const;
	Matrix3x3 operator*(double k) const;
	Vector3D operator*(const Vector3D& v) const;
	Matrix3x3 operator-() const;
	void operator+=(const Matrix3x3& m);
	void operator-=(const Matrix3x3& m);
	void operator*=(double k);
	bool operator==(const Matrix3x3& m) const;
	bool operator!=(const Matrix3x3& m) const;

	double* operator[](size_t i);

	void print() const;
public:
	double** m_mat;
};
