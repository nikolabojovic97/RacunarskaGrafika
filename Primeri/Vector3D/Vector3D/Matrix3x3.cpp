#include "Matrix3x3.h"
#include <math.h>

using namespace std;

Matrix3x3::Matrix3x3(){ 
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			Matrix[i][j] = 0;
}
Matrix3x3::Matrix3x3(double x11, double x12, double x13, double x21, double x22, double x23, double x31, double x32, double x33) {
	Set(x11, x12, x13, x21, x22, x23, x31, x32, x33);
}

Matrix3x3 Matrix3x3::operator +(Matrix3x3& M) {
	Matrix3x3 A;
	A.Copy(*this);

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			A.Matrix[i][j] += M.Matrix[i][j];
	return A;
}
void Matrix3x3::operator +=(Matrix3x3& M) {
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			Matrix[i][j] += M.Matrix[i][j];
}
Matrix3x3 Matrix3x3::operator -(Matrix3x3& M) {
	Matrix3x3 A;
	A.Copy(*this);

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			A.Matrix[i][j] -= M.Matrix[i][j];
	return A;
}
void Matrix3x3::operator -=(Matrix3x3& M) {
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			Matrix[i][j] -= M.Matrix[i][j];
}

Matrix3x3 Matrix3x3::operator *(Matrix3x3& M) {
	return Matrix3x3(
		Matrix[0][0] * M.Matrix[0][0] + Matrix[0][1] * M.Matrix[1][0] + Matrix[0][2] * M.Matrix[2][0],
		Matrix[0][0] * M.Matrix[0][1] + Matrix[0][1] * M.Matrix[1][1] + Matrix[0][2] * M.Matrix[2][1],
		Matrix[0][0] * M.Matrix[0][2] + Matrix[0][1] * M.Matrix[1][2] + Matrix[0][2] * M.Matrix[2][2],
		Matrix[1][0] * M.Matrix[0][0] + Matrix[1][1] * M.Matrix[1][0] + Matrix[1][2] * M.Matrix[2][0],
		Matrix[1][0] * M.Matrix[0][1] + Matrix[1][1] * M.Matrix[1][1] + Matrix[1][2] * M.Matrix[2][1],
		Matrix[1][0] * M.Matrix[0][2] + Matrix[1][1] * M.Matrix[1][2] + Matrix[1][2] * M.Matrix[2][2],
		Matrix[2][0] * M.Matrix[0][0] + Matrix[2][1] * M.Matrix[1][0] + Matrix[2][2] * M.Matrix[2][0],
		Matrix[2][0] * M.Matrix[0][1] + Matrix[2][1] * M.Matrix[1][1] + Matrix[2][2] * M.Matrix[2][1],
		Matrix[2][0] * M.Matrix[0][2] + Matrix[2][1] * M.Matrix[1][2] + Matrix[2][2] * M.Matrix[2][2]
	);
}

Vector3D Matrix3x3::operator *(Vector3D& A) {
	return Vector3D(
		Matrix[0][0] * A.x + Matrix[0][1] * A.y + Matrix[0][2] * A.z,
		Matrix[1][0] * A.x + Matrix[1][1] * A.y + Matrix[1][2] * A.z,
		Matrix[2][0] * A.x + Matrix[2][1] * A.y + Matrix[2][2] * A.z
	);
}

Matrix3x3 Matrix3x3::operator *(double k) {
	Matrix3x3 M;
	M.Copy(*this);

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			M.Matrix[i][j] *= k;
	return M;
}
void Matrix3x3::operator *=(double k) {
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			Matrix[i][j] *= k;
}

void Matrix3x3::LoadTranslate(double dx, double dy) {
	Set(1, 0, 0, 0, 1, 0, 0, 0, 1);
	Matrix[0][2] = dx;
	Matrix[1][2] = dy;
}
void Matrix3x3::LoadScale(double sx, double sy) {
	Set(0, 0, 0, 0, 0, 0, 0, 0, 1);
	Matrix[0][0] = sx;
	Matrix[1][1] = sy;
}
void Matrix3x3::LoadRotateAroundOrigin(double alpha) {
	Set(0, 0, 0, 0, 0, 0, 0, 0, 1);
	Matrix[0][0] = cos(alpha);
	Matrix[0][1] = -sin(alpha);
	Matrix[1][0] = sin(alpha);
	Matrix[1][1] = cos(alpha);
}

void Matrix3x3::Print() {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++)
			cout << " " << Matrix[i][j] << " ";
		cout << "\n";
	}
}

