#ifndef MATRIX4X4_H
#define MATRIX4X4_H

#include "Vector4D.h"
#include <assert.h>

using namespace std;

#define EPSILON 1.0e-10

enum ProjectionType { ORTHOGRAPHIC_PROJECTION, PERSPECTIVE_PROJECTION };

class Matrix4x4
{
public:
	double Matrix[4][4];
public:
	Matrix4x4();
	Matrix4x4(double x11, double x12, double x13, double x14,
		double x21, double x22, double x23, double x24,
		double x31, double x32, double x33, double x34,
		double x41, double x42, double x43, double x44);
	Matrix4x4(Matrix4x4 &A);
	Matrix4x4(const Matrix4x4& A);

	void Set(double x11, double x12, double x13, double x14,
		double x21, double x22, double x23, double x24,
		double x31, double x32, double x33, double x34,
		double x41, double x42, double x43, double x44) {
		Matrix[0][0] = x11;
		Matrix[0][1] = x12;
		Matrix[0][2] = x13;
		Matrix[0][3] = x14;
		Matrix[1][0] = x21;
		Matrix[1][1] = x22;
		Matrix[1][2] = x23;
		Matrix[1][3] = x24;
		Matrix[2][0] = x31;
		Matrix[2][1] = x32;
		Matrix[2][2] = x33;
		Matrix[2][3] = x34;
		Matrix[3][0] = x41;
		Matrix[3][1] = x42;
		Matrix[3][2] = x43;
		Matrix[3][3] = x44;
	}

	void Copy(Matrix4x4& М) {
		Set(
			М.Matrix[0][0],
			М.Matrix[0][1],
			М.Matrix[0][2],
			М.Matrix[0][3],
			М.Matrix[1][0],
			М.Matrix[1][1],
			М.Matrix[1][2],
			М.Matrix[1][3],
			М.Matrix[2][0],
			М.Matrix[2][1],
			М.Matrix[2][2],
			М.Matrix[2][3],
			М.Matrix[3][0],
			М.Matrix[3][1],
			М.Matrix[3][2],
			М.Matrix[3][3]
		);
	}
	void Copy(const Matrix4x4& М) {
		Set(
			М.Matrix[0][0],
			М.Matrix[0][1],
			М.Matrix[0][2],
			М.Matrix[0][3],
			М.Matrix[1][0],
			М.Matrix[1][1],
			М.Matrix[1][2],
			М.Matrix[1][3],
			М.Matrix[2][0],
			М.Matrix[2][1],
			М.Matrix[2][2],
			М.Matrix[2][3],
			М.Matrix[3][0],
			М.Matrix[3][1],
			М.Matrix[3][2],
			М.Matrix[3][3]
		);
	}

	void Zero();
	void Identity();
	void Neg();
	void Abs();
	Matrix4x4 Transpose();

	double Det();
	void Adjoint();
	bool Inv();

	bool Cmp(Matrix4x4& A);
	bool operator ==(Matrix4x4& A);
	bool operator !=(Matrix4x4& A);

	inline void set(int row, int col, double value);

	Matrix4x4 operator +(Matrix4x4& M);
	void operator +=(Matrix4x4& M);
	Matrix4x4 operator -(Matrix4x4& M);
	void operator -=(Matrix4x4& M);

	Matrix4x4 operator *(Matrix4x4& M);
	void operator *=(Matrix4x4& M);
	Vector4D operator *(Vector4D& V);

	Matrix4x4 operator *(double k);
	void operator *=(double k);

	void LoadTranslate(double dx, double dy, double dz);
	void LoadScale(double sx, double sy, double sz);
	void LoadRotateX(double alpha);
	void LoadRotateY(double alpha);
	void LoadRotateZ(double alpha);

	void LoadRotateAboutAxisThroughOrigin(const Vector4D &V, double phi);

	void Print();
};

inline void Matrix4x4::set(int row, int col, double value){
	assert(row >= 0 && row < 4 && col >= 0 && col < 4);
	Matrix[row][col] = value;
}

#endif

