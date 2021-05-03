#include "Matrix4x4.h"
#include <iostream>

Matrix4x4::Matrix4x4() {
	Set(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}
Matrix4x4::Matrix4x4(double x11, double x12, double x13, double x14,
	double x21, double x22, double x23, double x24,
	double x31, double x32, double x33, double x34,
	double x41, double x42, double x43, double x44) {
	Set(x11, x12, x13, x14, x21, x22, x23, x24, x31, x32, x33, x34, x41, x42, x43, x44);
}
Matrix4x4::Matrix4x4(Matrix4x4& A){
	Copy(A);
}
Matrix4x4::Matrix4x4(const Matrix4x4& A) {
	Copy(A);
}

void Matrix4x4::Zero(){
	Set(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}

void Matrix4x4::Identity(){
	Zero();
	for(int i = 0; i < 4; i++)
			Matrix[i][i] = 1;
}

void Matrix4x4::Neg(){
	*this*=-1;
}

void Matrix4x4::Abs(){
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (Matrix[i][j] < 0)
				Matrix[i][j] = -Matrix[i][j];
}

void Matrix4x4::Transpose() {
	double t;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			t = Matrix[i][j];
			Matrix[i][j] = Matrix[j][i];
			Matrix[j][i] = t;
		}
}
static inline double Det2x2(double a, double b, double c, double d){
	return((a * d) - (b * c));
}

static double Det3x3(double a1, double a2, double a3,
	double b1, double b2, double b3,
	double c1, double c2, double c3){
	return(a1 * Det2x2(b2, b3, c2, c3) -
		b1 * Det2x2(a2, a3, c2, c3) +
		c1 * Det2x2(a2, a3, b2, b3)
		);
}
double Matrix4x4::Det()
{
	double a1, a2, a3, a4, b1, b2, b3, b4, c1, c2, c3, c4, d1, d2, d3, d4;

	a1 = Matrix[0][0];
	b1 = Matrix[1][0];
	c1 = Matrix[2][0];
	d1 = Matrix[3][0];
	a2 = Matrix[0][1];
	b2 = Matrix[1][1];
	c2 = Matrix[2][1];
	d2 = Matrix[3][1];
	a3 = Matrix[0][2];
	b3 = Matrix[1][2];
	c3 = Matrix[2][2];
	d3 = Matrix[3][2];
	a4 = Matrix[0][3];
	b4 = Matrix[1][3];
	c4 = Matrix[2][3];
	d4 = Matrix[3][3];
	return(a1 * Det3x3(b2, b3, b4, c2, c3, c4, d2, d3, d4) -
		b1 * Det3x3(a2, a3, a4, c2, c3, c4, d2, d3, d4) +
		c1 * Det3x3(a2, a3, a4, b2, b3, b4, d2, d3, d4) -
		d1 * Det3x3(a2, a3, a4, b2, b3, b4, c2, c3, c4)
		);
}

void Matrix4x4::Adjoint()
{
	double a1, a2, a3, a4, b1, b2, b3, b4, c1, c2, c3, c4, d1, d2, d3, d4;

	a1 = Matrix[0][0];
	b1 = Matrix[1][0];
	c1 = Matrix[2][0];
	d1 = Matrix[3][0];
	a2 = Matrix[0][1];
	b2 = Matrix[1][1];
	c2 = Matrix[2][1];
	d2 = Matrix[3][1];
	a3 = Matrix[0][2];
	b3 = Matrix[1][2];
	c3 = Matrix[2][2];
	d3 = Matrix[3][2];
	a4 = Matrix[0][3];
	b4 = Matrix[1][3];
	c4 = Matrix[2][3];
	d4 = Matrix[3][3];
	Matrix[0][0] = Det3x3(b2, b3, b4, c2, c3, c4, d2, d3, d4);
	Matrix[0][1] = -Det3x3(a2, a3, a4, c2, c3, c4, d2, d3, d4);
	Matrix[0][2] = Det3x3(a2, a3, a4, b2, b3, b4, d2, d3, d4);
	Matrix[0][3] = -Det3x3(a2, a3, a4, b2, b3, b4, c2, c3, c4);
	Matrix[1][0] = -Det3x3(b1, b3, b4, c1, c3, c4, d1, d3, d4);
	Matrix[1][1] = Det3x3(a1, a3, a4, c1, c3, c4, d1, d3, d4);
	Matrix[1][2] = -Det3x3(a1, a3, a4, b1, b3, b4, d1, d3, d4);
	Matrix[1][3] = Det3x3(a1, a3, a4, b1, b3, b4, c1, c3, c4);
	Matrix[2][0] = Det3x3(b1, b2, b4, c1, c2, c4, d1, d2, d4);
	Matrix[2][1] = -Det3x3(a1, a2, a4, c1, c2, c4, d1, d2, d4);
	Matrix[2][2] = Det3x3(a1, a2, a4, b1, b2, b4, d1, d2, d4);
	Matrix[2][3] = -Det3x3(a1, a2, a4, b1, b2, b4, c1, c2, c4);
	Matrix[3][0] = -Det3x3(b1, b2, b3, c1, c2, c3, d1, d2, d3);
	Matrix[3][1] = Det3x3(a1, a2, a3, c1, c2, c3, d1, d2, d3);
	Matrix[3][2] = -Det3x3(a1, a2, a3, b1, b2, b3, d1, d2, d3);
	Matrix[3][3] = Det3x3(a1, a2, a3, b1, b2, b3, c1, c2, c3);
}
bool Matrix4x4::Inv()
{
	double determinant = Det();

	if (determinant < EPSILON)
		return false;

	Matrix4x4 A(*this);

	A.Adjoint();
	A *= determinant;

	Copy(A);

	return true;
}

bool Matrix4x4::Cmp(Matrix4x4& A) {
	double sub;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++) {
			sub = Matrix[i][j] - A.Matrix[i][j];
			if (sub < 0)
				sub = -sub;
			if (sub < EPSILON)
				return false;
		}

	return true;
}


Matrix4x4 Matrix4x4::operator +(Matrix4x4& M) {
	Matrix4x4 A;
	A.Copy(*this);
	A += M;
	return A;
}

void Matrix4x4::operator +=(Matrix4x4& M) {
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			Matrix[i][j] += M.Matrix[i][j];
}

Matrix4x4 Matrix4x4::operator -(Matrix4x4& M) {
	Matrix4x4 A;
	A.Copy(*this);
	A -= M;
	return A;
}

void Matrix4x4::operator -=(Matrix4x4& M) {
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			Matrix[i][j] -= M.Matrix[i][j];
}

Matrix4x4 Matrix4x4::operator *(Matrix4x4& M) {
	Matrix4x4 A;
	A.Copy(*this);
	A *= M;
	return A;
}

void Matrix4x4::operator *=(Matrix4x4& M) {
	Matrix4x4 A;

	double sum;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			sum = 0.0;
			for (int k = 0; k < 4; k++)
				sum += Matrix[i][k] * M.Matrix[k][j];

			A.Matrix[i][j] = sum;
		}
	}
	*this = A;
}


Vector4D Matrix4x4::operator *(Vector4D& V) {
	Vector4D A(
		Matrix[0][0] * V.x + Matrix[0][1] * V.y + Matrix[0][2] * V.z + Matrix[0][3] * V.t,
		Matrix[1][0] * V.x + Matrix[1][1] * V.y + Matrix[1][2] * V.z + Matrix[1][3] * V.t,
		Matrix[2][0] * V.x + Matrix[2][1] * V.y + Matrix[2][2] * V.z + Matrix[2][3] * V.t,
		Matrix[3][0] * V.x + Matrix[3][1] * V.y + Matrix[3][2] * V.z + Matrix[3][3] * V.t
		);
	return A;
}

Matrix4x4 Matrix4x4::operator *(double k) {
	Matrix4x4 A;
	A.Copy(*this);
	A *= k;
	return A;
}

void Matrix4x4::operator *=(double k) {
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			Matrix[i][j] *= k;
}

bool Matrix4x4::operator ==(Matrix4x4& A) {
	return Cmp(A);
}

bool Matrix4x4::operator !=(Matrix4x4& A) {
	return !Cmp(A);
}




//Funkcije za tranformacije
void Matrix4x4::LoadTranslate(double tx, double ty, double tz) {
	Identity();
	Matrix[0][3] = tx;
	Matrix[1][3] = ty;
	Matrix[2][3] = tz;
}

void Matrix4x4::LoadScale(double sx, double sy, double sz) {
	Identity();
	Matrix[0][0] = sx;
	Matrix[1][1] = sy;
	Matrix[2][2] = sz;
}

void Matrix4x4::LoadRotateX(double alpha) {
	double sinAlpha = sin(alpha);
	double cosAlpha = cos(alpha);

	Identity();
	Matrix[1][1] = cosAlpha;
	Matrix[1][2] = -sinAlpha;
	Matrix[2][1] = sinAlpha;
	Matrix[2][2] = cosAlpha;
}

void Matrix4x4::LoadRotateY(double alpha) {
	double sinAlpha = sin(alpha);
	double cosAlpha = cos(alpha);

	Identity();
	Matrix[0][0] = cosAlpha;
	Matrix[0][2] = -sinAlpha;
	Matrix[2][0] = sinAlpha;
	Matrix[2][2] = cosAlpha;
}

void Matrix4x4::LoadRotateZ(double alpha) {
	double sinAlpha = sin(alpha);
	double cosAlpha = cos(alpha);

	Identity();
	Matrix[0][0] = cos(alpha);
	Matrix[0][1] = -sin(alpha);
	Matrix[1][0] = sin(alpha);
	Matrix[1][1] = cos(alpha);
}

void Matrix4x4::LoadRotateAboutAxisThroughOrigin(const Vector4D &V, double phi)
{
	Zero();
	Matrix[3][3] = 1.0;

	double cos_phi = cos(phi);
	double sin_phi = sin(phi);
	double t = 1.0 - cos_phi;

	Vector4D VN(V);
	VN.Normalize();

	Matrix[0][0] = VN.X()*VN.X() * t + cos_phi;
	Matrix[0][1] = VN.X()*VN.Y() * t - VN.Z()*sin_phi;
	Matrix[0][2] = VN.X()*VN.Z() * t + VN.Y()*sin_phi;

	Matrix[1][0] = VN.X()*VN.Y() * t + VN.Z()*sin_phi;
	Matrix[1][1] = VN.Y()*VN.Y() * t + cos_phi;
	Matrix[1][2] = VN.Y()*VN.Z() * t - VN.X()*sin_phi;
	
	Matrix[2][0] = VN.X()*VN.Z() * t - VN.Y()*sin_phi;
	Matrix[2][1] = VN.Y()*VN.Z() * t + VN.X()*sin_phi;
	Matrix[2][2] = VN.Z()*VN.Z() * t + cos_phi;
}

void Matrix4x4::Print() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++)
			cout << " " << Matrix[i][j] << " ";
		cout << "\n";
	}
}
