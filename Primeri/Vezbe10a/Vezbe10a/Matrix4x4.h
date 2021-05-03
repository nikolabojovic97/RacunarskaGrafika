#pragma once
#include <vector>
#include "Vector3D.h"
#include "Vector4D.h"
#define EPS 10e-8

class Matrix4x4
{
public:
	Matrix4x4();
	Matrix4x4(const Matrix4x4& m);

	void toZero();
	void toIdentity();
	void abs();
	void transpose();

	void loadTranslate(const Vector3D& v);
	void loadTranslate(const Vector4D& v);
	void loadTranslate(double tx, double ty, double tz);
	void loadScale(double sx, double sy, double sz);
	void loadRotateX(double phi);
	void loadRotateY(double phi);
	void loadRotateZ(double phi);
	void loadRotateAxis(const Vector4D& axis, double angle);
	void loadRotateAxis(const Vector4D& axis, double angle, const Vector4D& origin);
	void loadMatchVector(const Vector4D& from, const Vector4D& to);
	void loadMatchVector(const Vector4D& from, const Vector4D& to, const Vector4D& origin);

	void transform(Vector4D& v) const;
	void transform(std::vector<Vector4D>& vectors) const;
	Vector4D transformed(const Vector4D& v);
	std::vector<Vector4D> transformed(const std::vector<Vector4D>& vectors);

	double det() const;

	Matrix4x4 operator+(const Matrix4x4& m) const;
	Matrix4x4 operator-(const Matrix4x4& m) const;
	Matrix4x4 operator*(const Matrix4x4& m) const;
	Matrix4x4 operator*(double k) const;
	Vector4D operator*(const Vector4D& v) const;
	Matrix4x4 operator-() const;
	void operator+=(const Matrix4x4& m);
	void operator-=(const Matrix4x4& m);
	void operator*=(double k);
	bool operator==(const Matrix4x4& m) const;
	bool operator!=(const Matrix4x4& m) const;

	double* operator[](size_t i);

	void print() const;
public:
	double** m_mat;
};
