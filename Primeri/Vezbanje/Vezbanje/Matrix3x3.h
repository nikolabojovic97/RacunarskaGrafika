#pragma once
#include "Vector3D.h"
#ifndef MATRIX3X3_H
#define MATRIX3X3_H

using namespace std;

class Matrix3x3
{
public:
	double Matrix[3][3];
public:
	Matrix3x3();
	Matrix3x3(double x11, double x12, double x13, double x21, double x22, double x23, double x31, double x32, double x33);

	void Set(double x11, double x12, double x13, double x21, double x22, double x23, double x31, double x32, double x33) {
		Matrix[0][0] = x11;
		Matrix[0][1] = x12;
		Matrix[0][2] = x13;
		Matrix[1][0] = x21;
		Matrix[1][1] = x22;
		Matrix[1][2] = x23;
		Matrix[2][0] = x31;
		Matrix[2][1] = x32;
		Matrix[2][2] = x33;
	}

	void Copy(Matrix3x3& M) {
		Set(
			M.Matrix[0][0],
			M.Matrix[0][1],
			M.Matrix[0][2],
			M.Matrix[1][0],
			M.Matrix[1][1],
			M.Matrix[1][2],
			M.Matrix[2][0],
			M.Matrix[2][1],
			M.Matrix[2][2]
		);
	}

	Matrix3x3 operator +(Matrix3x3& M);
	void operator +=(Matrix3x3& M);
	Matrix3x3 operator -(Matrix3x3& M);
	void operator -=(Matrix3x3& M);

	Matrix3x3 operator *(Matrix3x3& M);
	Vector3D operator *(Vector3D& A);
	
	Matrix3x3 operator *(double k);
	void operator *=(double k);

	void LoadTranslate(double dx, double dy);
	void LoadScale(double sx, double sy);
	void LoadRotateAroundOrigin(double alpha);

	void Print();
};
#endif

